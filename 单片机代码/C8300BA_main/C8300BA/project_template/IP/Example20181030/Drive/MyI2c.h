#ifndef __MYIIC_H
#define __MYIIC_H
#include "ht32.h"
//////////////////////////////////////////////////////////////////////////////////	 
//								  
//////////////////////////////////////////////////////////////////////////////////
   	   		   
#define  SDA_PORT         HT_GPIOA      
#define  SCK_PORT         HT_GPIOA

#define SDA_PIN       (GPIO_PIN_1)
#define SCK_PIN       (GPIO_PIN_0)

               
//IO方向设置
//#define SDA_IN()  {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}
#define SDA_IN() 	{GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_1, GPIO_DIR_IN);GPIO_InputConfig(HT_GPIOA, GPIO_PIN_1, ENABLE);}
										
//#define SDA_OUT() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}
#define SDA_OUT()	{GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_1, GPIO_DIR_OUT);GPIO_InputConfig(HT_GPIOA, GPIO_PIN_1, DISABLE);}


//IO操作函数	 
//#define IIC_SCL    PCout(12) //SCL
//#define IIC_SDA    PCout(11) //SDA	 
//#define READ_SDA   PCin(11)  //输入SDA 

extern u8 Si1141_I2CRxbuffer[13];
extern u8 Si1141_I2CTxbuffer[13];
//IIC所有操作函数
//void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
//u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Read_Byte(void);				//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

//extern void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
extern u8 IIC_WriteOneByte(u8 WriteAddr,u8 DataToWrite);

//extern void IIC_WriteLenByte(u8 WriteAddr,u32 DataToWrite,u8 Len);
//extern u8 IIC_WriteLenByte(u8 WriteAddr,u32 DataToWrite,u8 Len);
extern u8 IIC_WriteLenByte(u8 WriteAddr,u8 *values,u8 Len);

//extern u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	
extern int8_t IIC_ReadOneByte(u8 ReadAddr);
//extern u32 IIC_ReadLenByte(u8 ReadAddr,u8 Len);
extern int8_t IIC_ReadLenByte(u8 ReadAddr,u8 Len);
void delay_us(u16 delaycnt);
#endif


