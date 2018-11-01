#include "Si114x_functions.h"
#include "ht32.h"
#include "ht32_board.h"
#include "MyI2c.h"

int16_t Si114xInit(void)
{
    GPIO_DirectionConfig(SCK_PORT, SCK_PIN, GPIO_DIR_OUT);
    
//    Si114xWriteToRegister( si114x_handle, 0x08, 0x00); 
    IIC_WriteOneByte(0x08,0x00);
//    Si114xWriteToRegister( si114x_handle, 0x09, 0x00); 
    IIC_WriteOneByte(0x09, 0x00); 
    Si114xReset(); 
//    System_Delay_Ms(10); 
//    Si114xWriteToRegister( si114x_handle, 0x07, 0x17); 
    IIC_WriteOneByte(0x07, 0x17); 
//		Si114xWriteToRegister( si114x_handle, 0x0f, 0x06);  电流90ma
    IIC_WriteOneByte(0x0f, 0x06); 		//电流202ma
	
	
  /*  Si114xParamSet( 0x0b, 0x00); 		//PS ADC  gain LED PULSE 25.6us
    Si114xParamSet( 0x0c, 0x04); 
    Si114xParamSet( 0x02, 0x01); 
    Si114xParamSet( 0x07, 0x00); 
    Si114xParamSet( 0x08, 0x00); 
    Si114xParamSet( 0x09, 0x00); 
    Si114xParamSet( 0x12, 0x00); 
    Si114xParamSet( 0x11, 0x00); 
    Si114xParamSet( 0x1f, 0x00); 
    Si114xParamSet( 0x1e, 0x00); 
    Si114xParamSet( 0x0e, 0x00); 
    Si114xParamSet( 0x01, 0x31); */
		
    Si114xParamSet(  0x0b, 0x01); 		//PS ADC  gain LED PULSE 51.2us
    Si114xParamSet(  0x0c, 0x04); 
    Si114xParamSet(  0x02, 0x01); 
    Si114xParamSet(  0x07, 0x03); 
    Si114xParamSet(  0x08, 0x00); 
    Si114xParamSet(  0x09, 0x00); 
    Si114xParamSet(  0x12, 0x00); 
    Si114xParamSet(  0x11, 0x01); 
    Si114xParamSet(  0x1f, 0x00); 
    Si114xParamSet(  0x1e, 0x01); 
    Si114xParamSet(  0x0e, 0x03); 
    Si114xParamSet(  0x01, 0x31); 
    IIC_WriteOneByte(0x03, 0x01);			//中断开启
    IIC_WriteOneByte(0x04, 0x01d);		//PS1 VIS IR INT ALL ENABLE
    IIC_WriteOneByte(0x05, 0x00);
//    Si114xWriteToRegister( si114x_handle, 0x06, 0x00); 
    IIC_WriteOneByte(0x06, 0x00);
//    Si114xWriteToRegister( si114x_handle, 0x08, 0x94); 
    IIC_WriteOneByte(0x08, 0xb9);				//10.4MS  sample frequency	
//    Si114xWriteToRegister( si114x_handle, 0x09, 0x08); 
    IIC_WriteOneByte(0x09, 0x08);
//    Si114xWriteToRegister( si114x_handle, 0x0a, 0x08); 
    IIC_WriteOneByte(0x0a, 0x08);
    Si114xPsAlsAuto();		//ps vis ir auto 
//		Si114xPsAuto();
    return 0;
}


