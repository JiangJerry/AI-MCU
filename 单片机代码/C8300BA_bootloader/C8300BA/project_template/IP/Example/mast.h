#ifndef   MAST_H
#define   MAST_H
typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;
#define   SYSCLK_48MHz
#define   HTCFG_MCTM_RELOAD       (24000)  // 48000000/24000=2000HZ
#define   HTCFG_MCTM_DEAD_TIME    0
#define   SW1_KEY                 1
#define   SW1_KEY_L               11

#define   KEY_LONG_TIME           25
#define   KEY_SHORT_TIME          1
#define   KEY_SHAKE_TIME          80

#define   LED_EN_GPIO             (HT_GPIOB)
#define   LED_EN_GPIO_ID          (GPIO_PB)
#define   LED_EN_GPIO_PIN         (GPIO_PIN_7)
#define   LED_EN_AFIO_MODE        (AFIO_FUN_MCTM_GPTM)

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

#define   LED_EN_TM_CHANNEL       (TM_CH_1)

#define   LED_EN_Co_On            (HT_GPIOA->DOUTR)|=LED_EN_GPIO_PIN    
#define   LED_EN_Co_Off           (HT_GPIOA->DOUTR)&=~((u32)LED_EN_GPIO_PIN)

#define   READ_SDA                 (HT_GPIOC->DINR&IIC_SDA_GPIO_PIN)

#define   LIGHT                   0x00
#define   MOVE_MD                 0x01
#define   FLASHING                0x02

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
#define   MS_1_Sign                Flag.bits.b1

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

extern u16       LED_PWM1_Duty;
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
#endif
