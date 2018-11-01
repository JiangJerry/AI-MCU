#ifndef   MAST_H
#define   MAST_H
typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;

#define true 1
    #define false 0

#define   SYSCLK_48MHz
#define   HTCFG_MCTM_RELOAD       (1920)  // 48000000/1920=25000HZ
#define   HTCFG_MCTM_DEAD_TIME    0
#define   SW1_KEY                 1
#define   SW1_KEY_L               11

#define   KEY_LONG_TIME           25
#define   KEY_SHORT_TIME          1
#define   KEY_SHAKE_TIME          80


#define   LED_RED_GPIO             (HT_GPIOB)
#define   LED_RED_GPIO_ID          (GPIO_PB)
#define   LED_RED_GPIO_PIN         (GPIO_PIN_4)
#define   LED_RED_AFIO_MODE        (AFIO_MODE_1)

#define   LED_RED_OFF            (LED_RED_GPIO->DOUTR)|=LED_RED_GPIO_PIN    
#define   LED_RED_ON           (LED_RED_GPIO->DOUTR)&=~((u32)LED_RED_GPIO_PIN)
#define   LED_RED_FLICK         (LED_RED_GPIO->DOUTR) ^= ((u32)LED_RED_GPIO_PIN)

#define   LED_GREEN_GPIO             (HT_GPIOB)
#define   LED_GREEN_GPIO_ID          (GPIO_PB)
#define   LED_GREEN_GPIO_PIN         (GPIO_PIN_3)
#define   LED_GREEN_AFIO_MODE        (AFIO_MODE_1)

#define   LED_GREEN_OFF            (LED_GREEN_GPIO->DOUTR)|=LED_GREEN_GPIO_PIN    
#define   LED_GREEN_ON           (LED_GREEN_GPIO->DOUTR)&=~((u32)LED_GREEN_GPIO_PIN)
#define   LED_GREEN_FLICK         (LED_GREEN_GPIO->DOUTR) ^= ((u32)LED_GREEN_GPIO_PIN)


#define   LED_EN1_GPIO             (HT_GPIOA)
#define   LED_EN1_GPIO_ID          (GPIO_PA)
#define   LED_EN1_GPIO_PIN         (GPIO_PIN_15)
#define   LED_EN1_AFIO_MODE        (AFIO_MODE_1)

#define   LED_EN2_GPIO             (HT_GPIOB)
#define   LED_EN2_GPIO_ID          (GPIO_PB)
#define   LED_EN2_GPIO_PIN         (GPIO_PIN_0)
#define   LED_EN2_AFIO_MODE        (AFIO_MODE_1)


#define   LED_EN3_GPIO             (HT_GPIOA)
#define   LED_EN3_GPIO_ID          (GPIO_PA)
#define   LED_EN3_GPIO_PIN         (GPIO_PIN_14)
#define   LED_EN3_AFIO_MODE        (AFIO_MODE_1)


#define   LED_EN4_GPIO             (HT_GPIOB)
#define   LED_EN4_GPIO_ID          (GPIO_PB)
#define   LED_EN4_GPIO_PIN         (GPIO_PIN_1)
#define   LED_EN4_AFIO_MODE        (AFIO_MODE_1)





#define   LED_PWM_GPIO             (HT_GPIOB)
#define   LED_PWM_GPIO_ID          (GPIO_PB)
#define   LED_PWM_GPIO_PIN         (GPIO_PIN_7)
#define   LED_PWM_AFIO_MODE        (AFIO_FUN_MCTM_GPTM)


#define   UR1_COM_TX_GPIO            (HT_GPIOA)
#define   UR1_COM_TX_GPIO_ID         (GPIO_PA)
#define   UR1_COM_TX_AFIO_PIN        (AFIO_PIN_4)
#define   UR1_COM_TX_AFIO_MODE       (AFIO_FUN_USART_UART)
#define   UR1_COM_RX_GPIO            (HT_GPIOA)
#define   UR1_COM_RX_GPIO_ID         (GPIO_PA)
#define   UR1_COM_RX_AFIO_PIN        (AFIO_PIN_5)
#define   UR1_COM_RX_AFIO_MODE       (AFIO_FUN_USART_UART)
#define   UR1_COM_PORT               HT_UART1

#define   UR0_WF_COM_TX_GPIO         (HT_GPIOC)
#define   UR0_WF_COM_TX_GPIO_ID      (GPIO_PC)
#define   UR0_WF_COM_TX_AFIO_PIN     (AFIO_PIN_6)
#define   UR0_WF_COM_TX_AFIO_MODE    (AFIO_FUN_USART_UART)
#define   UR0_WF_COM_RX_GPIO         (HT_GPIOC)
#define   UR0_WF_COM_RX_GPIO_ID      (GPIO_PC)
#define   UR0_WF_COM_RX_AFIO_PIN     (AFIO_PIN_7)
#define   UR0_WF_COM_RX_AFIO_MODE    (AFIO_FUN_USART_UART)
#define   UR0_WF_COM_PORT             HT_UART0

#define   SW1_GPIO                (HT_GPIOB)
#define   SW1_GPIO_ID             (GPIO_PB)
#define   SW1_GPIO_PIN            (GPIO_PIN_9)
#define   SW1_AFIO_MODE           (AFIO_FUN_GPIO)
#define   SW1_DI                  (HT_GPIOB->DINR&SW1_GPIO_PIN)

#define   BAT_STA_GPIO                (HT_GPIOA)
#define   BAT_STA_GPIO_ID             (GPIO_PA)
#define   BAT_STA_GPIO_PIN            (GPIO_PIN_6)
#define   BAT_STA_AFIO_MODE           (AFIO_FUN_GPIO)
#define   BAT_STA_DI                  (HT_GPIOA->DINR&BAT_STA_GPIO_PIN)


#define   WIFI_EN_GPIO              (HT_GPIOB)
#define   WIFI_EN_GPIO_ID           (GPIO_PB)
#define   WIFI_EN_GPIO_PIN          (GPIO_PIN_10)
#define   WIFI_EN_AFIO_MODE         (AFIO_FUN_GPIO)
#define   WIFI_EN_DI                (HT_GPIOB->DINR&SW1_GPIO_PIN)

#define  OPEN_WIFI_POWER            GPIO_WriteOutBits(WIFI_EN_GPIO, WIFI_EN_GPIO_PIN, SET);	   
#define  CLOSE_WIFI_POWER            GPIO_WriteOutBits(WIFI_EN_GPIO, WIFI_EN_GPIO_PIN, RESET);	   


#define   LED_EN_TM_CHANNEL       (TM_CH_1)

#define   LED_EN1_ON            (LED_EN1_GPIO->DOUTR)|=LED_EN1_GPIO_PIN    
#define   LED_EN1_OFF           (LED_EN1_GPIO->DOUTR)&=~((u32)LED_EN1_GPIO_PIN)

#define   LED_EN2_ON            (LED_EN2_GPIO->DOUTR)|=LED_EN2_GPIO_PIN    
#define   LED_EN2_OFF           (LED_EN2_GPIO->DOUTR)&=~((u32)LED_EN2_GPIO_PIN)

#define   LED_EN3_ON            (LED_EN3_GPIO->DOUTR)|=LED_EN3_GPIO_PIN    
#define   LED_EN3_OFF           (LED_EN3_GPIO->DOUTR)&=~((u32)LED_EN3_GPIO_PIN)

#define   LED_EN4_ON            (LED_EN4_GPIO->DOUTR)|=LED_EN4_GPIO_PIN    
#define   LED_EN4_OFF           (LED_EN4_GPIO->DOUTR)&=~((u32)LED_EN4_GPIO_PIN)


#define   READ_SDA                 (HT_GPIOC->DINR&IIC_SDA_GPIO_PIN)

#define   LIGHT                   0x00
#define   MOVE_MD                 0x01
#define   FLASHING                0x02


typedef struct
{
    u8 OnOrOff;
    u8 Brightness;
    u8 CoolDuty;
    u8 Color;
    u16 BrtAllValu;
    u16 WarmLoadValu;
    u16 WarmLastLoadValu;
    u16 CoolLoadValu;
    u16 CoolLastLoadValu;
}_LED_CTRL;
extern _LED_CTRL LedCtrl;

typedef union 
{ 
	unsigned char byte;
  	struct 
  	{
	  	unsigned	char b0:1; 
	  	unsigned	char b1:1;
	  	unsigned	char b2:1; 
	  	unsigned	char b3:1;     
	    unsigned 	char b4:1; 
	    unsigned 	char b5:1; 
	    unsigned 	char b6:1;
	    unsigned 	char b7:1;       
    }bits;
}Flag_Byte;
extern    volatile Flag_Byte       Flag;
#define   MS_25_Sign               Flag.bits.b0
#define   Time1msFlag              Flag.bits.b1
#define     Time500msFlag           Flag.bits.b2
typedef struct 
{
    u8        Tx_Buf[25]; 
    u8        Rx_Buf[25]; 
    u8        Rx_Buf_Invalid[25];
    u8        Receive_Bytes;
    u8        Receive_Sign;
    u8        Send_Step;
    u8        Send_Cmd;
    u8        MS_1_Receive;
    u8        MS_10_Count;
    u8        Receive_Bytes_Temp;
    u8        Receive_Enable;
}Communicate; 	  	
extern Communicate Ur0_WF,Ur1;

extern u16 LEDCoolReloadValu;
extern u16 LEDWarmReloadValu;
extern u8        Key_CntL;
extern u8        Key_CntH;   
extern u8        Key_Value;
extern u8        Key_Temp;
extern u8        Key_Temp_Temp;
extern u8        Key;
extern u8        Workmode;
extern u8        MS_1_Count;
extern u8        MS_25_Count;
extern u8        Device_Buf;

void Ram_Communicate_Clr(Communicate *Ptr);
void My_Ram_Init(void);
void GPIO_Init(void);
void HT32_BFTM0_Init(void);
void LVD_Init(void);
void HT32_MCTM0_PWM_Init(void);
void Clock_Config(void);
void SysClockConfig_48MHz(void);
void Load_Clr(void);
void WDG_Init(void);
void Uart_Or_Usart_Init(void);
void Key_Scan(void);
void Function(void);
void Ur0_WF_Receive_Function(void);
void Ur1_Receive_Function(void);
void Systime_Updata_Function(void);
void Uart_Deal(void);
void Device_Function(void);
void  GPTM0_PWMInit(void);
void SetColor(u8 Color);
void SetColorWarm(u8 ColorWarm);
void SetBrightness(u8 Brt);
#endif
