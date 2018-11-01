#include "ht32.h"
#include "ht32_board.h"
#include "mast.h"
#include "uart_protocal.h"

Communicate Ur0_WF,Ur1;
volatile  Flag_Byte      Flag;

//u16 LEDCoolReloadValu = 0;
//u16 LEDWarmReloadValu = 0;
u8        Key_CntL;
u8        Key_CntH;   
u8        Key_Value;
u8        Key_Temp;
u8        Key_Temp_Temp;
u8        Key;
u8        Workmode;
u8        MS_1_Count;
u8        MS_25_Count;
u8        Device_Buf;

extern u8 onoff;

const u8  Key_Table[2]={0x00,0x01};

void Ram_Communicate_Clr(Communicate *Ptr)
{
    u8 i;
    for(i=0;i<25;i++)
    {
        Ptr->Tx_Buf[i]=0;
        Ptr->Rx_Buf[i]=0;
        Ptr->Rx_Buf_Invalid[i]=0;
    }
    Ptr->Receive_Bytes=0;
    Ptr->Receive_Sign=0;
    Ptr->Send_Step=0;
    Ptr->Send_Cmd=0;
    Ptr->MS_1_Receive=0;
    Ptr->MS_10_Count=0;
    Ptr->Receive_Bytes_Temp=0;
    Ptr->Receive_Enable=0;
}

void HT32_BFTM0_Init(void)
{
    BFTM_DeInit(HT_BFTM0);
    BFTM_SetCompare(HT_BFTM0, SystemCoreClock/1000); 
    BFTM_SetCounter(HT_BFTM0, 0);
    BFTM_IntConfig(HT_BFTM0, ENABLE);
    BFTM_EnaCmd(HT_BFTM0, ENABLE);
    BFTM_IntConfig(HT_BFTM0,ENABLE);
    NVIC_EnableIRQ(BFTM0_IRQn);
}

void LVD_Init(void)
{
    while (PWRCU_CheckReadyAccessed() != PWRCU_OK);
    PWRCU_BODRISConfig(PWRCU_BODRIS_RESET);
    PWRCU_BODCmd(ENABLE);
    PWRCU_SetLVDS(PWRCU_LVDS_2V7);
    PWRCU_LVDCmd(ENABLE);
}


void Clock_Config(void)
{
    #ifdef SYSCLK_8MHz
    SysClockConfig_8MHz();
    #elif defined SYSCLK_24MHz
    SysClockConfig_24MHz();
    #elif defined SYSCLK_40MHz
    SysClockConfig_40MHz();
    #elif defined SYSCLK_48MHz
    SysClockConfig_48MHz();
    #endif
}

void SysClockConfig_48MHz(void)
{
    CKCU_PLLInitTypeDef PLLInit;
    HT_CKCU->AHBCFGR = 0;                          /* Reset value of AHBCFGR                                  */
    HT_CKCU->AHBCCR = 0x00000065;                  /* Reset value of AHBCCR                                   */
    HT_CKCU->APBCFGR = 0x00010000;                 /* Reset value of APBCFGR                                  */
    HT_CKCU->APBCCR0 = 0;                          /* Reset value of APBCCR0                                  */
    HT_CKCU->APBCCR1 = 0;                          /* Reset value of APBCCR1                                  */
    HT_CKCU->APBPCSR0 = 0;                         /* Reset value of APBPCSR0                                 */
    HT_CKCU->APBPCSR1 = 0;                         /* Reset value of APBPCSR1                                 */
    HT_CKCU->LPCR = 0;                             /* Reset value of LPCR                                     */
    HT_CKCU->MCUDBGCR = 0;                         /* Reset value of MCUDBGCR                                 */
    CKCU_HSICmd(ENABLE);
    while (CKCU_GetClockReadyStatus(CKCU_FLAG_HSIRDY) == RESET) ;
    CKCU_SysClockConfig(CKCU_SW_HSI);
    PLLInit.ClockSource = CKCU_PLLSRC_HSI;
    PLLInit.CFG = CKCU_PLL_8M_48M;
    PLLInit.BYPASSCmd = DISABLE;
    CKCU_PLLInit(&PLLInit);
    CKCU_PLLCmd(ENABLE);
    while(CKCU_GetClockReadyStatus(CKCU_FLAG_PLLRDY) == RESET);
    FLASH_SetWaitState(FLASH_WAITSTATE_1);  /* FLASH zero wait clock */
    CKCU_SetHCLKPrescaler(CKCU_SYSCLK_DIV1);
    while(CKCU_SysClockConfig(CKCU_SW_PLL)!=SUCCESS);
}

void My_Ram_Init(void)
{
    Ram_Communicate_Clr(&Ur0_WF);
    Ram_Communicate_Clr(&Ur1);
    LedCtrl.CoolLoadValu = 0;
    LedCtrl.WarmLoadValu = 0;
    Key_CntL=0;
    Key_CntH=0;   
    Key_Value=0;
    Key_Temp=50;
    Key=50;
    Workmode=0;
    MS_1_Count=0;
    MS_25_Count=0;
    Device_Buf=0;
    Flag.byte=0x00;
    init_uart_buffer();	
}


void WDG_Init(void)
{
    //WDT_DeInit();
    /* Set Prescaler Value, 32K/64 = 500 Hz                                                                   */
    WDT_SetPrescaler(WDT_PRESCALER_64);
    /* Set Prescaler Value, 500 Hz/50 = 10 Hz                                                            */
    WDT_SetReloadValue(50);
    WDT_Restart();                    // Reload Counter as WDTV Value
    WDT_SetDeltaValue(50);          // Set Delta Value
    WDT_ProtectCmd(ENABLE);           // Enable Protection    DISABLE
    WDT_Cmd(ENABLE);
}

void Uart_Or_Usart_Init(void)
{
    USART_InitTypeDef USART_InitStructure;
    AFIO_GPxConfig(UR0_WF_COM_TX_GPIO_ID, UR0_WF_COM_TX_AFIO_PIN, AFIO_FUN_USART_UART);
    AFIO_GPxConfig(UR0_WF_COM_RX_GPIO_ID, UR0_WF_COM_RX_AFIO_PIN, AFIO_FUN_USART_UART);
    AFIO_GPxConfig(UR1_COM_TX_GPIO_ID, UR1_COM_TX_AFIO_PIN, AFIO_FUN_USART_UART);
    AFIO_GPxConfig(UR1_COM_RX_GPIO_ID, UR1_COM_RX_AFIO_PIN, AFIO_FUN_USART_UART);
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
    USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
    USART_InitStructure.USART_Parity = USART_PARITY_NO;
    USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
    USART_Init(UR0_WF_COM_PORT, &USART_InitStructure);   
    USART_IntConfig(UR0_WF_COM_PORT, USART_INT_RXDR, ENABLE);
    USART_TxCmd(UR0_WF_COM_PORT, ENABLE);
    USART_RxCmd(UR0_WF_COM_PORT, ENABLE);
    NVIC_EnableIRQ(UART0_IRQn);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
    USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
    USART_InitStructure.USART_Parity = USART_PARITY_NO;
    USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
    USART_Init(UR1_COM_PORT, &USART_InitStructure);   
    USART_IntConfig(UR1_COM_PORT, USART_INT_RXDR, ENABLE);
    USART_TxCmd(UR1_COM_PORT, ENABLE);
    USART_RxCmd(UR1_COM_PORT, ENABLE);
    NVIC_EnableIRQ(UART1_IRQn);
}

void Key_Scan(void)
{
    u8 Key_Buf,i;
    Key_Buf=0;
    if(!(SW1_DI)) Key_Buf|=0x01;
    for(i=0;i<2;i++)
    {
        if(Key_Buf==Key_Table[i]) break;
    }
    if(i==0)
    {
        if(Key_CntH>=KEY_SHORT_TIME&&(Key_CntH<KEY_LONG_TIME)) 
        {
            Key=Key_Temp;	  
        }
        Key_CntH=0;
        Key_CntL=0;
        Key_Temp=50;
    }
    if(Key_Value==i)
    {
        Key_CntL++;
        if(Key_CntL>=KEY_SHAKE_TIME)
        {
            Key_CntL=0;
            if(Key_CntH<(KEY_LONG_TIME+1)) Key_CntH++;
            if(Key_CntH==KEY_SHORT_TIME) 
            {
                    Key_Temp=i;
            }	
            if(Key_CntH==KEY_LONG_TIME) Key=i+10;
        }
    }
    else 
    {
        Key_CntH=0;
        Key_CntL=0;
    }
    Key_Value=i;
}

void Function(void)
{
    if(Key==SW1_KEY)
    {
        //USART_SendData(UR0_WF_COM_PORT,Key);
        //Turn On/off Light
        if(LedCtrl.OnOrOff == 1)
        {
            LedCtrl.OnOrOff = 0;
        }
        else
        {
            LedCtrl.OnOrOff = 1;
        }
        TurnOnOff_Light(LedCtrl.OnOrOff);
        upload_light_onoff(LedCtrl.OnOrOff,0x00);
            
    }
    else if(Key == SW1_KEY_L )
    {
        SetWiFiToSmlink();//Set WIFI to SMLINK status
    }
    Key=50;
}

void Ur0_WF_Receive_Function(void)
{
    int check=1;
    if(Ur0_WF.Receive_Bytes_Temp>0)
    {            
        Push_data(&g_uart0,Ur0_WF.Rx_Buf,Ur0_WF.Receive_Bytes_Temp);
        while(check)
        {
            check = ParaseUartFrame(&g_uart0);
        }
        Ur0_WF.Receive_Bytes_Temp=0;
    }
}

void Ur1_Receive_Function(void)
{
    u8 i;
    if(Ur1.Receive_Bytes_Temp>0)
    {
        Ur1.Rx_Buf_Invalid[i]=Ur1.Rx_Buf[i];
        USART_SendData(UR1_COM_PORT,Ur1.Rx_Buf_Invalid[i]);
        Ur1.Receive_Bytes_Temp=0;
    }
}

void UART1Send(u8 Data)
{
    USART_SendData(UR1_COM_PORT,Data);    
}

void Uart_Deal(void)
{
    if(Ur0_WF.Receive_Enable==1)
    {
        if(Ur0_WF.MS_1_Receive<255) 
            Ur0_WF.MS_1_Receive++;
    }
    else Ur0_WF.MS_1_Receive=0;
    
    if(Ur0_WF.MS_1_Receive>99) 
    {
        Ur0_WF.Receive_Bytes_Temp=Ur0_WF.Receive_Bytes;
        Ur0_WF.Receive_Bytes=0;
        Ur0_WF.MS_1_Receive=0;
        Ur0_WF.Receive_Enable=0;
    }
    if(Ur1.Receive_Enable==1)
    {
        if(Ur1.MS_1_Receive<255) Ur1.MS_1_Receive++;
    }
    else Ur1.MS_1_Receive=0;
    
    if(Ur1.MS_1_Receive>99) 
    {
        Ur1.Receive_Bytes_Temp=Ur1.Receive_Bytes;
        Ur1.Receive_Bytes=0;
        Ur1.MS_1_Receive=0;
        Ur1.Receive_Enable=0;
    }	
}

