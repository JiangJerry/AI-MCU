#include "si114x_functions.h"
#include "ht32.h"
#include "ht32_board.h"
#include "MyI2c.h"

void System_Delay_Ms(u8 Cnt)
{
    Cnt = Cnt + 100;
    for(;Cnt>0;Cnt--);
}

#define LOOP_TIMEOUT_MS 200
/***************************************************************************//**
 * @brief 
 *   Waits until the Si113x/4x is sleeping before proceeding
 ******************************************************************************/
static int16_t _waitUntilSleep(void)
{
  int8_t  retval = -1;
  uint8_t count = 0;
  // This loops until the Si114x is known to be in its sleep state
  // or if an i2c error occurs
  while(count < LOOP_TIMEOUT_MS)
  {
    //retval = Si114xReadFromRegister(si114x_handle, REG_CHIP_STAT);
    retval = IIC_ReadOneByte(REG_CHIP_STAT);
		if(retval == 1) break;
    if(retval <  0) return retval;
    count++;
   //delay_1ms();
		System_Delay_Ms(1);
  }
  return 0;
}

/***************************************************************************//**
 * @brief 
 *   Resets the Si113x/4x, clears any interrupts and initializes the HW_KEY 
 *   register.
 * @param[in] si114x_handle 
 *   The programmer's toolkit handle
 * @retval  0       
 *   Success
 * @retval  <0      
 *   Error
 ******************************************************************************/
int16_t Si114xReset(void)
{
  int32_t retval = 0;

  //
  // Do not access the Si114x earlier than 25 ms from power-up. 
  // Uncomment the following lines if Si114xReset() is the first
  // instruction encountered, and if your system MCU boots up too 
  // quickly. 
  //
//  delay_10ms();
 // delay_10ms();
 // delay_10ms();c
	System_Delay_Ms(10);
	System_Delay_Ms(10);

  //retval+=Si114xWriteToRegister(si114x_handle, REG_MEAS_RATE,  0x00);
    retval+=IIC_WriteOneByte(REG_MEAS_RATE,	0x00);
  //retval+=Si114xWriteToRegister(si114x_handle, REG_ALS_RATE,   0x00);
    retval+=IIC_WriteOneByte(REG_ALS_RATE,  0x00);
    retval+=Si114xPauseAll();

// The clearing of the registers could be redundant, but it is okay.
// This is to make sure that these registers are cleared.
//  retval+=Si114xWriteToRegister(si114x_handle, REG_MEAS_RATE,  0x00);
//  retval+=Si114xWriteToRegister(si114x_handle, REG_IRQ_ENABLE, 0x00);
//  retval+=Si114xWriteToRegister(si114x_handle, REG_IRQ_MODE1,  0x00);
//  retval+=Si114xWriteToRegister(si114x_handle, REG_IRQ_MODE2,  0x00);
//  retval+=Si114xWriteToRegister(si114x_handle, REG_INT_CFG  ,  0x00);
//  retval+=Si114xWriteToRegister(si114x_handle, REG_IRQ_STATUS, 0xFF);
	
    retval+=IIC_WriteOneByte(REG_MEAS_RATE,  0x00);
    retval+=IIC_WriteOneByte(REG_IRQ_ENABLE, 0x00);
    retval+=IIC_WriteOneByte(REG_IRQ_MODE1,  0x00);
    retval+=IIC_WriteOneByte(REG_IRQ_MODE2,  0x00);
    retval+=IIC_WriteOneByte(REG_INT_CFG  ,  0x00);
    retval += IIC_WriteOneByte(REG_IRQ_STATUS, 0xFF);

  // Perform the Reset Command
  //retval+=Si114xWriteToRegister(si114x_handle, REG_COMMAND, 1);
    retval+=IIC_WriteOneByte(REG_COMMAND, 1);

  // Delay for 10 ms. This delay is needed to allow the Si114x
  // to perform internal reset sequence. 
  //delay_10ms();
	System_Delay_Ms(10);

  // Write Hardware Key
  //retval+=Si114xWriteToRegister(si114x_handle, REG_HW_KEY, HW_KEY_VAL0);
  retval+=IIC_WriteOneByte(REG_HW_KEY, HW_KEY_VAL0);

  return retval;
}


void ClearIRQ(void)
{
    IIC_WriteOneByte(REG_IRQ_STATUS, 0xFF);
}
/***************************************************************************//**
 * @brief 
 *   Helper function to send a command to the Si113x/4x
 ******************************************************************************/
//static int16_t _sendCmd(HANDLE si114x_handle, uint8_t command)
static int16_t _sendCmd(uint8_t command)
{
  int16_t  response; 
  int8_t   retval;
  uint8_t  count = 0; 

  // Get the response register contents
  //response = Si114xReadFromRegister(si114x_handle, REG_RESPONSE);
	response = IIC_ReadOneByte(REG_RESPONSE);
  if(response < 0)
    return response;

  // Double-check the response register is consistent
  while(count < LOOP_TIMEOUT_MS)
  {
    if((retval=_waitUntilSleep()) != 0) return retval;

    if(command==0) break; // Skip if the command is NOP 

    //retval=Si114xReadFromRegister(si114x_handle, REG_RESPONSE);
		retval=IIC_ReadOneByte(REG_RESPONSE);
    if(retval==response) break;
    else if(retval<0) return retval;
    else response = retval;
    count++;
  }

  // Send the Command
  //if((retval=Si114xWriteToRegister(si114x_handle, REG_COMMAND, command)) !=0) 
	if((retval=IIC_WriteOneByte(REG_COMMAND, command)) !=0) 
    return retval;

  count = 0;
  // Expect a change in the response register
  while(count < LOOP_TIMEOUT_MS)
  {
    if(command==0) break; // Skip if the command is NOP

    //retval= Si114xReadFromRegister(si114x_handle, REG_RESPONSE);
		retval= IIC_ReadOneByte(REG_RESPONSE);
    if(retval != response) break;
    else if(retval<0) return retval;
    count++;
    //delay_1ms();
		System_Delay_Ms(1);
  }
  return 0;
}
/***************************************************************************//**
 * @brief 
 *   Sends a PSALSAUTO command to the Si113x/4x
 * @param[in] si114x_handle
 *   The programmer's toolkit handle
 * @retval  0       
 *   Success
 * @retval  <0      
 *   Error
 ******************************************************************************/
int16_t Si114xPsAlsAuto(void)
{
  return _sendCmd(0x0F);
}

/***************************************************************************/
/***************************************************************************//**
 * @brief 
 *   Sends a PSAUTO command to the Si113x/4x
 * @param[in] si114x_handle
 *   The programmer's toolkit handle
 * @retval  0       
 *   Success
 * @retval  <0      
 *   Error
 ******************************************************************************/
int16_t Si114xPsAuto(void)
{
  return _sendCmd(0x0D);
}

/***************************************************************************/
/***************************************************************************//**
 * @brief 
 *   Sends a NOP command to the Si113x/4x
 * @param[in] si114x_handle
 *   The programmer's toolkit handle
 * @retval  0       Success
 * @retval  <0      Error
 ******************************************************************************/
int16_t Si114xNop(void)
{
  return _sendCmd(0x00);
}

/***************************************************************************//**
 * @brief 
 *   Sends a PSFORCE command to the Si113x/4x
 * @param[in] si114x_handle
 *   The programmer's toolkit handle
 * @retval  0       
 *   Success
 * @retval  <0      
 *   Error
 ******************************************************************************/
int16_t Si114xPsForce(void)
{
  return _sendCmd(0x05);
}

/***************************************************************************//**
 * @brief 
 *   Sends an ALSFORCE command to the Si113x/4x
 * @param[in] si114x_handle
 *   The programmer's toolkit handle
 * @retval  0       
 *   Success
 * @retval  <0      
 *   Error
 ******************************************************************************/
int16_t Si114xAlsForce(void)
{
  return _sendCmd(0x06);
}

/***************************************************************************//**
 * @brief 
 *   Sends a PSALSFORCE command to the Si113x/4x
 * @param[in] si114x_handle
 *   The programmer's toolkit handle
 * @retval  0       
 *   Success
 * @retval  <0      
 *   Error
 ******************************************************************************/
int16_t Si114xPsAlsForce(void)
{
  return _sendCmd(0x07);
}

/***************************************************************************//**
 * @brief 
 *   Reads a Parameter from the Si113x/4x
 * @param[in] si114x_handle 
 *   The programmer's toolkit handle
 * @param[in] address   
 *   The address of the parameter. 
 * @retval <0       
 *   Error
 * @retval 0-255    
 *   Parameter contents
 ******************************************************************************/
int16_t Si114xParamRead(uint8_t address)
{
  // returns Parameter[address]
  int16_t retval;
  uint8_t cmd = 0x80 + (address & 0x1F);

  retval=_sendCmd(cmd);
  if( retval != 0 ) return retval;

  //retval = Si114xReadFromRegister(si114x_handle, REG_PARAM_RD);
	retval = IIC_ReadOneByte(REG_PARAM_RD);
  return retval;
}

/***************************************************************************//**
 * @brief 
 *   Writes a byte to an Si113x/4x Parameter
 * @param[in] si114x_handle 
 *   The programmer's toolkit handle
 * @param[in] address 
 *   The parameter address 
 * @param[in] value   
 *   The byte value to be written to the Si113x/4x parameter
 * @retval 0    
 *   Success
 * @retval <0   
 *   Error
 * @note This function ensures that the Si113x/4x is idle and ready to
 * receive a command before writing the parameter. Furthermore, 
 * command completion is checked. If setting parameter is not done
 * properly, no measurements will occur. This is the most common
 * error. It is highly recommended that host code make use of this
 * function.
 ******************************************************************************/
//int16_t Si114xParamSet(HANDLE si114x_handle, uint8_t address, uint8_t value)
int16_t Si114xParamSet(uint8_t address, uint8_t value)
{
    int16_t retval;
    uint8_t buffer[2];
    int16_t response_stored;
    int16_t response;

    if((retval = _waitUntilSleep())!=0) return retval;

    //  response_stored = Si114xReadFromRegister(si114x_handle, REG_RESPONSE);
    response_stored = IIC_ReadOneByte(REG_RESPONSE);

    buffer[0]= value;
    buffer[1]= 0xA0 + (address & 0x1F);

    //  retval=Si114xBlockWrite(si114x_handle, REG_PARAM_WR, 2, ( uint8_t* ) buffer);
    //retval=Si114xBlockWrite(REG_PARAM_WR,( uint8_t* ) buffer,2);
    retval = IIC_WriteLenByte(REG_PARAM_WR,( uint8_t* ) buffer,2);
    if(retval != 0) return retval;

    // Wait for command to finish
    response = IIC_ReadOneByte(REG_RESPONSE);
    while(response == response_stored )
    {
    response = IIC_ReadOneByte(REG_RESPONSE);
    if (response == response_stored)
    {
      //delay_1ms();
            System_Delay_Ms(1);
    }
    }

    if(retval < 0)
    return retval;
    else
    return 0;
}

/***************************************************************************//**
 * @brief 
 *   Pause measurement helper function
 ******************************************************************************/
static int16_t _PsAlsPause (void) 
{
  return _sendCmd(0x0B);
}

u8 ReadPS(u8 Register)
{
    return(IIC_ReadOneByte(Register));
}
/***************************************************************************//**
 * @brief 
 *   Pauses autonomous measurements
 * @param[in] si114x_handle 
 *  The programmer's toolkit handle
 * @retval  0       
 *   Success
 * @retval  <0      
 *   Error
 ******************************************************************************/
int16_t Si114xPauseAll(void)
{
  uint8_t countA, countB;
  int8_t  retval;


  //  After a RESET, if the Si114x receives a command (including NOP) before the
  //  Si114x has gone to sleep, the chip hangs. This first while loop avoids 
  //  this.  The reading of the REG_CHIPSTAT does not disturb the internal MCU.
  //

   retval = 0; //initialize data so that we guarantee to enter the loop
   while(retval != 0x01)
   {    
     //retval = Si114xReadFromRegister( si114x_handle, REG_CHIP_STAT);
     retval = IIC_ReadOneByte(REG_CHIP_STAT);
     if (retval != 0x01)
     {
       //delay_1ms();
        System_Delay_Ms(1);
     }
   }

  countA = 0;
  while(countA < LOOP_TIMEOUT_MS)
  {
    countB = 0;
    // Keep sending nops until the response is zero
    while(countB < LOOP_TIMEOUT_MS)
    {
      //retval = Si114xReadFromRegister(si114x_handle, REG_RESPONSE);
			retval = IIC_ReadOneByte(REG_RESPONSE);
      if( retval == 0 )
          break;
      else 
      {
        // Send the NOP Command to clear any error...we cannot use Si114xNop()
        // because it first checks if REG_RESPONSE < 0 and if so it does not
        // perform the cmd. Since we have a saturation REG_RESPONSE will be <0
        IIC_WriteOneByte(REG_COMMAND, 0x00);
      }
      countB++;
      //delay_1ms();
			System_Delay_Ms(1);
    }

    // Pause the device
    _PsAlsPause();

    countB = 0;
    // Wait for response
    while(countB < LOOP_TIMEOUT_MS)
    {
      //retval = Si114xReadFromRegister(si114x_handle, REG_RESPONSE);
			retval = IIC_ReadOneByte(REG_RESPONSE);
      if( retval !=0 )
        break;
      countB++;
      //delay_1ms();
			System_Delay_Ms(1);
    }

    // When the PsAlsPause() response is good, we expect it to be a '1'.
    //retval = Si114xReadFromRegister(si114x_handle, REG_RESPONSE);
		retval = IIC_ReadOneByte(REG_RESPONSE);
    if( retval == 1 )
      break;  // otherwise, start over.
    countA++;
  }
  return 0;
}



