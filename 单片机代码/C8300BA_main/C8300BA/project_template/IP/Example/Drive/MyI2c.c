///* Includes ------------------------------------------------------------------------------------------------*/
//#include "ht32.h"
//#include "ht32_board.h"
//#include "MyI2c.h"

////在AT24CXX指定地址读出一个数据
////ReadAddr:开始读数的地址  
////返回值  :读到的数据
//#define Readcmd 0x01
//#define Writecmd 0x00
//#define SlaveAdress 0x5a
//#define GolbalAdress 0x00
//#define GolbalResetAdress 0x06


//u8 Si1141_I2CRxbuffer[13]={0};
//u8 Si1141_I2CTxbuffer[13]={0};
/////*****************************************************************/
//int8_t IIC_ReadOneByte(u8 ReadAddr)
//{				  
//	u8 temp=0;	
//	int8_t retcnt=0;
//    IIC_Start();  
////	if(EE_TYPE>AT24C16)
////	{
//		IIC_Send_Byte((SlaveAdress <<1) | Writecmd);	   //发送写命令
//		retcnt += IIC_Wait_Ack();
//		IIC_Send_Byte(ReadAddr);//发送高地址	    
/////	}else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	   
//	retcnt += IIC_Wait_Ack(); 
////  IIC_Send_Byte(ReadAddr%256);   //发送低地址
////	IIC_Wait_Ack();	    
//	IIC_Start();  	 	   
//	IIC_Send_Byte((SlaveAdress <<1) | Readcmd);           //进入接收模式			   
//	retcnt += IIC_Wait_Ack();	
//    temp=IIC_Read_Byte();	
//    IIC_NAck();//发送nACK	   
//    IIC_Stop();//产生一个停止条件	   
//	if(retcnt>=1)		//0表示成功
//	{
//		retcnt = -1;	//表示读错误
//		return retcnt;
//	}
//	else
//	{
//		return temp;
//	}
//}
////在AT24CXX指定地址写入一个数据
////WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
//u8 IIC_WriteOneByte(u8 WriteAddr,u8 DataToWrite)
//{			
//    u8 temp=0;
//    IIC_Start();  
//    //	if(EE_TYPE>AT24C16)
//    //	{
//        IIC_Send_Byte((SlaveAdress <<1) | Writecmd);	    //发送写命令
//        temp += IIC_Wait_Ack();
//        IIC_Send_Byte(WriteAddr);//发送写入地址   
//    temp += IIC_Wait_Ack();	  	 										  		   
//    IIC_Send_Byte(DataToWrite);     //发送字节							   
//    temp += IIC_Wait_Ack();  		    	   
//    IIC_Stop();		//产生一个停止条件 
//	return temp;
//}

////在AT24CXX里面的指定地址开始写入长度为Len的数据
////该函数用于写入16bit或者32bit的数据.
////WriteAddr  :开始写入的地址  
////DataToWrite:数据数组首地址
////Len        :要写入数据的长度2,4
//u8 IIC_WriteLenByte(u8 WriteAddr,u8 *values,u8 Len)
//{  	
//	u8 t;
//	u8 i=0;
//	IIC_Start();  
//	IIC_Send_Byte((SlaveAdress <<1) | Writecmd);	    //发送写命令
//	i += IIC_Wait_Ack();
//	IIC_Send_Byte(WriteAddr);//发送写入起始地址	   
//	i += IIC_Wait_Ack();	   								  		   		    	   
//	for(t=0;t<Len;t++)
//	{
////		IIC_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
//		IIC_Send_Byte(values[t]);     //发送字节							   
//		i += IIC_Wait_Ack();
//	}
//  IIC_Stop();		//产生一个停止条件
//		return i;
//} 

////在AT24CXX里面的指定地址开始读出长度为Len的数据
////该函数用于读出16bit或者32bit的数据.
////ReadAddr   :开始读出的地址 
////返回值     :数据
////Len        :要读出数据的长度2,4
//int8_t IIC_ReadLenByte(u8 ReadAddr,u8 Len)
//{  	
//	u8 t;
////	u32 temp=0;	    
//	int8_t retcnt=0;
//  IIC_Start();  
//	IIC_Send_Byte((SlaveAdress <<1) | Writecmd);	   //发送写命令
//	IIC_Wait_Ack();
//	IIC_Send_Byte(ReadAddr);//发送读起始地址	    
//	IIC_Wait_Ack();  
//	IIC_Start();  	 	   
//	IIC_Send_Byte((SlaveAdress <<1) | Readcmd);           //进入接收模式			   
//	IIC_Wait_Ack();	 
////  temp=IIC_Read_Byte(0);		   
////  IIC_Stop();//产生一个停止条件	    
////	return temp;
//	for(t=0;t<Len;t++)
//	{
////		temp<<=8;
////		temp+=IIC_ReadOneByte(ReadAddr+Len-t-1); 	
//		Si1141_I2CRxbuffer[t]=IIC_Read_Byte();
////		Si1141_I2CRxbuffer[Len-t]=IIC_ReadOneByte(ReadAddr+Len-t-1);
//		if(t==Len-1)
//        IIC_NAck();//发送NACK
//    else
//        IIC_Ack(); //发送ACK
//	}
//	IIC_Stop();//		发送停止命令
//	if(retcnt>=1)		//0表示成功
//	{
//		retcnt = -1;	//1表示失败
//		return retcnt;
//	}
//	else
//	{
//		return retcnt;	
//	}   											    
//}

////检查AT24CXX是否正常
////这里用了24XX的最后一个地址(255)来存储标志字.
////如果用其他24C系列,这个地址要修改
////返回1:检测失败
////返回0:检测成功
//u8 IIC_Check(void)
//{
//	u8 temp;
//	temp=IIC_ReadOneByte(255);//避免每次开机都写AT24CXX			   
//	if(temp==0X55)return 0;		   
//	else//排除第一次初始化的情况
//	{
//		IIC_WriteOneByte(255,0X55);
//	    temp=IIC_ReadOneByte(255);	  
//		if(temp==0X55)return 0;
//	}
//	return 1;											  
//}

////在AT24CXX里面的指定地址开始读出指定个数的数据
////ReadAddr :开始读出的地址 对24c02为0~255
////pBuffer  :数据数组首地址
////NumToRead:要读出数据的个数
//void IIC_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
//{
//	while(NumToRead)
//	{
//		*pBuffer++=IIC_ReadOneByte(ReadAddr++);	
//		NumToRead--;
//	}
//}  

////在AT24CXX里面的指定地址开始写入指定个数的数据
////WriteAddr :开始写入的地址 对24c02为0~255
////pBuffer   :数据数组首地址
////NumToWrite:要写入数据的个数
//void IIC_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
//{
//	while(NumToWrite--)
//	{
//		IIC_WriteOneByte(WriteAddr,*pBuffer);
//		WriteAddr++;
//		pBuffer++;
//	}
//}

////产生IIC起始信号
//void IIC_Start(void)
//{
//	SDA_OUT();     //sda线输出
////	IIC_SDA=1;	
//	GPIO_SetOutBits(HT_GPIOA, GPIO_PIN_1);	//SET SDA  	  
////	IIC_SCL=1;
//	GPIO_SetOutBits(HT_GPIOA, GPIO_PIN_0);	//SET SCL
//	delay_us(20);
// //	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
//	GPIO_ClearOutBits(HT_GPIOA, GPIO_PIN_1);	//CLEAR SDA
//	delay_us(20);
////	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
//	GPIO_ClearOutBits(HT_GPIOA, GPIO_PIN_0);	//CLEAR SCL
//}	  
////产生IIC停止信号
//void IIC_Stop(void)
//{
//	SDA_OUT();//sda线输出
////	IIC_SCL=0;
//	GPIO_ClearOutBits(HT_GPIOA, GPIO_PIN_0);	//CLEAR SCL
////	IIC_SDA=0;
//	GPIO_ClearOutBits(HT_GPIOA, GPIO_PIN_1);	//CLEAR SDA
// 	delay_us(20); 
////	IIC_SCL=1;//STOP:when CLK is high DATA change form low to high
//	GPIO_SetOutBits(HT_GPIOA, GPIO_PIN_0);	//SET SCL
// 	delay_us(20); 
////	IIC_SDA=1;//发送I2C总线结束信号 		
//	GPIO_SetOutBits(HT_GPIOA, GPIO_PIN_1);	//SET SDA
//}
////等待应答信号到来
////返回值：1，接收应答失败
////        0，接收应答成功
//u8 IIC_Wait_Ack(void)
//{
//	u8 ucErrTime=0;
//	SDA_IN();      //SDA设置为输入  
////	IIC_SDA=1;delay_us(1);	
//	GPIO_SetOutBits(HT_GPIOA, GPIO_PIN_1);	//SET SDA   
//	delay_us(5);
////	IIC_SCL=1;delay_us(1);
//	GPIO_SetOutBits(HT_GPIOA, GPIO_PIN_0);	//SET SCL	
//	delay_us(5);
////	while(READ_SDA)
//	while(GPIO_ReadInBit(HT_GPIOA,GPIO_PIN_1))
//	{
//		ucErrTime++;
//		if(ucErrTime>250)
//		{
//			IIC_Stop();
//			return 1;
//		}
//	}
////	IIC_SCL=0;//时钟输出0 	
//	GPIO_ClearOutBits(HT_GPIOA, GPIO_PIN_0);	//CLEAR SCL   
//	return 0;  
//} 
////产生ACK应答
//void IIC_Ack(void)
//{
////	IIC_SCL=0;
//	GPIO_ClearOutBits(HT_GPIOA, GPIO_PIN_0);	//CLEAR SCL
//	SDA_OUT();
////	IIC_SDA=0;
//	GPIO_ClearOutBits(HT_GPIOA, GPIO_PIN_1);	//CLEAR SDA
//	delay_us(10);
////	IIC_SCL=1;
//	GPIO_SetOutBits(HT_GPIOA, GPIO_PIN_0);	//SET SCL
//	delay_us(10);
////	IIC_SCL=0;
//	GPIO_ClearOutBits(HT_GPIOA, GPIO_PIN_0);	//CLEAR SCL
//}
////不产生ACK应答		    
//void IIC_NAck(void)
//{
////	IIC_SCL=0;
//	GPIO_ClearOutBits(HT_GPIOA, GPIO_PIN_0);	//CLEAR SCL
//	SDA_OUT();
////	IIC_SDA=1;
//	GPIO_SetOutBits(HT_GPIOA, GPIO_PIN_1);	//SET SDA 
//	delay_us(10);
////	IIC_SCL=1;
//	GPIO_SetOutBits(HT_GPIOA, GPIO_PIN_0);	//SET SCL
//	delay_us(10);
////	IIC_SCL=0;
//	GPIO_ClearOutBits(HT_GPIOA, GPIO_PIN_0);	//CLEAR SCL
//}					 				     
////IIC发送一个字节
////返回从机有无应答
////1，有应答
////0，无应答			  
//void IIC_Send_Byte(u8 txd)
//{                        
//    u8 t;   
//	SDA_OUT(); 	    
////    IIC_SCL=0;//拉低时钟开始数据传输
//		GPIO_ClearOutBits(HT_GPIOA, GPIO_PIN_0);	//CLEAR SCL
//    for(t=0;t<8;t++)
//    {              
//        //IIC_SDA=(txd&0x80)>>7;
//			if((txd&0x80)>>7)
//			{
//				GPIO_SetOutBits(HT_GPIOA, GPIO_PIN_1);	//SET SDA 
//			}
//			else
//			{
//				GPIO_ClearOutBits(HT_GPIOA, GPIO_PIN_1);	//CLEAR SDA
//			}
//        txd<<=1; 	  
////		delay_us(10);   //对TEA5767这三个延时都是必须的
////		IIC_SCL=1;
//			GPIO_SetOutBits(HT_GPIOA, GPIO_PIN_0);	//SET SCL
//			delay_us(10); 
////		IIC_SCL=0;	
//			GPIO_ClearOutBits(HT_GPIOA, GPIO_PIN_0);	//CLEAR SCL
//			delay_us(10);
//    }	 
//} 	    
////读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
////u8 IIC_Read_Byte(unsigned char ack)
//u8 IIC_Read_Byte(void)
//{
//	unsigned char i,receive=0;
//	SDA_IN();//SDA设置为输入
//  for(i=0;i<8;i++ )
//	{
//    //    IIC_SCL=0; 
//		GPIO_ClearOutBits(HT_GPIOA, GPIO_PIN_0);	//CLEAR SCL
//        delay_us(10);
//		//IIC_SCL=1;
//		GPIO_SetOutBits(HT_GPIOA, GPIO_PIN_0);	//SET SCL
//        receive<<=1;
//    //    if(READ_SDA)receive++;   
//		if(GPIO_ReadInBit(HT_GPIOA,GPIO_PIN_1))
//		{
//			receive++;
//		}
//		delay_us(5); 
//    }					 
////    if (!ack)
////        IIC_NAck();//发送nACK
////    else
////        IIC_Ack(); //发送ACK   
//    return receive;
//}

////延时函数 us级 
//void delay_us(u16 delaycnt)
//{
//	for(;delaycnt>0;delaycnt--)
//	{
//		__nop();
//	}
//}
