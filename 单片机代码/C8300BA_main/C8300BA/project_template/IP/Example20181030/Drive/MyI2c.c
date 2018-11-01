///* Includes ------------------------------------------------------------------------------------------------*/
//#include "ht32.h"
//#include "ht32_board.h"
//#include "MyI2c.h"

////��AT24CXXָ����ַ����һ������
////ReadAddr:��ʼ�����ĵ�ַ  
////����ֵ  :����������
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
//		IIC_Send_Byte((SlaveAdress <<1) | Writecmd);	   //����д����
//		retcnt += IIC_Wait_Ack();
//		IIC_Send_Byte(ReadAddr);//���͸ߵ�ַ	    
/////	}else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 	   
//	retcnt += IIC_Wait_Ack(); 
////  IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
////	IIC_Wait_Ack();	    
//	IIC_Start();  	 	   
//	IIC_Send_Byte((SlaveAdress <<1) | Readcmd);           //�������ģʽ			   
//	retcnt += IIC_Wait_Ack();	
//    temp=IIC_Read_Byte();	
//    IIC_NAck();//����nACK	   
//    IIC_Stop();//����һ��ֹͣ����	   
//	if(retcnt>=1)		//0��ʾ�ɹ�
//	{
//		retcnt = -1;	//��ʾ������
//		return retcnt;
//	}
//	else
//	{
//		return temp;
//	}
//}
////��AT24CXXָ����ַд��һ������
////WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
//u8 IIC_WriteOneByte(u8 WriteAddr,u8 DataToWrite)
//{			
//    u8 temp=0;
//    IIC_Start();  
//    //	if(EE_TYPE>AT24C16)
//    //	{
//        IIC_Send_Byte((SlaveAdress <<1) | Writecmd);	    //����д����
//        temp += IIC_Wait_Ack();
//        IIC_Send_Byte(WriteAddr);//����д���ַ   
//    temp += IIC_Wait_Ack();	  	 										  		   
//    IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
//    temp += IIC_Wait_Ack();  		    	   
//    IIC_Stop();		//����һ��ֹͣ���� 
//	return temp;
//}

////��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
////�ú�������д��16bit����32bit������.
////WriteAddr  :��ʼд��ĵ�ַ  
////DataToWrite:���������׵�ַ
////Len        :Ҫд�����ݵĳ���2,4
//u8 IIC_WriteLenByte(u8 WriteAddr,u8 *values,u8 Len)
//{  	
//	u8 t;
//	u8 i=0;
//	IIC_Start();  
//	IIC_Send_Byte((SlaveAdress <<1) | Writecmd);	    //����д����
//	i += IIC_Wait_Ack();
//	IIC_Send_Byte(WriteAddr);//����д����ʼ��ַ	   
//	i += IIC_Wait_Ack();	   								  		   		    	   
//	for(t=0;t<Len;t++)
//	{
////		IIC_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
//		IIC_Send_Byte(values[t]);     //�����ֽ�							   
//		i += IIC_Wait_Ack();
//	}
//  IIC_Stop();		//����һ��ֹͣ����
//		return i;
//} 

////��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
////�ú������ڶ���16bit����32bit������.
////ReadAddr   :��ʼ�����ĵ�ַ 
////����ֵ     :����
////Len        :Ҫ�������ݵĳ���2,4
//int8_t IIC_ReadLenByte(u8 ReadAddr,u8 Len)
//{  	
//	u8 t;
////	u32 temp=0;	    
//	int8_t retcnt=0;
//  IIC_Start();  
//	IIC_Send_Byte((SlaveAdress <<1) | Writecmd);	   //����д����
//	IIC_Wait_Ack();
//	IIC_Send_Byte(ReadAddr);//���Ͷ���ʼ��ַ	    
//	IIC_Wait_Ack();  
//	IIC_Start();  	 	   
//	IIC_Send_Byte((SlaveAdress <<1) | Readcmd);           //�������ģʽ			   
//	IIC_Wait_Ack();	 
////  temp=IIC_Read_Byte(0);		   
////  IIC_Stop();//����һ��ֹͣ����	    
////	return temp;
//	for(t=0;t<Len;t++)
//	{
////		temp<<=8;
////		temp+=IIC_ReadOneByte(ReadAddr+Len-t-1); 	
//		Si1141_I2CRxbuffer[t]=IIC_Read_Byte();
////		Si1141_I2CRxbuffer[Len-t]=IIC_ReadOneByte(ReadAddr+Len-t-1);
//		if(t==Len-1)
//        IIC_NAck();//����NACK
//    else
//        IIC_Ack(); //����ACK
//	}
//	IIC_Stop();//		����ֹͣ����
//	if(retcnt>=1)		//0��ʾ�ɹ�
//	{
//		retcnt = -1;	//1��ʾʧ��
//		return retcnt;
//	}
//	else
//	{
//		return retcnt;	
//	}   											    
//}

////���AT24CXX�Ƿ�����
////��������24XX�����һ����ַ(255)���洢��־��.
////���������24Cϵ��,�����ַҪ�޸�
////����1:���ʧ��
////����0:���ɹ�
//u8 IIC_Check(void)
//{
//	u8 temp;
//	temp=IIC_ReadOneByte(255);//����ÿ�ο�����дAT24CXX			   
//	if(temp==0X55)return 0;		   
//	else//�ų���һ�γ�ʼ�������
//	{
//		IIC_WriteOneByte(255,0X55);
//	    temp=IIC_ReadOneByte(255);	  
//		if(temp==0X55)return 0;
//	}
//	return 1;											  
//}

////��AT24CXX�����ָ����ַ��ʼ����ָ������������
////ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
////pBuffer  :���������׵�ַ
////NumToRead:Ҫ�������ݵĸ���
//void IIC_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
//{
//	while(NumToRead)
//	{
//		*pBuffer++=IIC_ReadOneByte(ReadAddr++);	
//		NumToRead--;
//	}
//}  

////��AT24CXX�����ָ����ַ��ʼд��ָ������������
////WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
////pBuffer   :���������׵�ַ
////NumToWrite:Ҫд�����ݵĸ���
//void IIC_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
//{
//	while(NumToWrite--)
//	{
//		IIC_WriteOneByte(WriteAddr,*pBuffer);
//		WriteAddr++;
//		pBuffer++;
//	}
//}

////����IIC��ʼ�ź�
//void IIC_Start(void)
//{
//	SDA_OUT();     //sda�����
////	IIC_SDA=1;	
//	GPIO_SetOutBits(HT_GPIOA, GPIO_PIN_1);	//SET SDA  	  
////	IIC_SCL=1;
//	GPIO_SetOutBits(HT_GPIOA, GPIO_PIN_0);	//SET SCL
//	delay_us(20);
// //	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
//	GPIO_ClearOutBits(HT_GPIOA, GPIO_PIN_1);	//CLEAR SDA
//	delay_us(20);
////	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
//	GPIO_ClearOutBits(HT_GPIOA, GPIO_PIN_0);	//CLEAR SCL
//}	  
////����IICֹͣ�ź�
//void IIC_Stop(void)
//{
//	SDA_OUT();//sda�����
////	IIC_SCL=0;
//	GPIO_ClearOutBits(HT_GPIOA, GPIO_PIN_0);	//CLEAR SCL
////	IIC_SDA=0;
//	GPIO_ClearOutBits(HT_GPIOA, GPIO_PIN_1);	//CLEAR SDA
// 	delay_us(20); 
////	IIC_SCL=1;//STOP:when CLK is high DATA change form low to high
//	GPIO_SetOutBits(HT_GPIOA, GPIO_PIN_0);	//SET SCL
// 	delay_us(20); 
////	IIC_SDA=1;//����I2C���߽����ź� 		
//	GPIO_SetOutBits(HT_GPIOA, GPIO_PIN_1);	//SET SDA
//}
////�ȴ�Ӧ���źŵ���
////����ֵ��1������Ӧ��ʧ��
////        0������Ӧ��ɹ�
//u8 IIC_Wait_Ack(void)
//{
//	u8 ucErrTime=0;
//	SDA_IN();      //SDA����Ϊ����  
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
////	IIC_SCL=0;//ʱ�����0 	
//	GPIO_ClearOutBits(HT_GPIOA, GPIO_PIN_0);	//CLEAR SCL   
//	return 0;  
//} 
////����ACKӦ��
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
////������ACKӦ��		    
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
////IIC����һ���ֽ�
////���شӻ�����Ӧ��
////1����Ӧ��
////0����Ӧ��			  
//void IIC_Send_Byte(u8 txd)
//{                        
//    u8 t;   
//	SDA_OUT(); 	    
////    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
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
////		delay_us(10);   //��TEA5767��������ʱ���Ǳ����
////		IIC_SCL=1;
//			GPIO_SetOutBits(HT_GPIOA, GPIO_PIN_0);	//SET SCL
//			delay_us(10); 
////		IIC_SCL=0;	
//			GPIO_ClearOutBits(HT_GPIOA, GPIO_PIN_0);	//CLEAR SCL
//			delay_us(10);
//    }	 
//} 	    
////��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
////u8 IIC_Read_Byte(unsigned char ack)
//u8 IIC_Read_Byte(void)
//{
//	unsigned char i,receive=0;
//	SDA_IN();//SDA����Ϊ����
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
////        IIC_NAck();//����nACK
////    else
////        IIC_Ack(); //����ACK   
//    return receive;
//}

////��ʱ���� us�� 
//void delay_us(u16 delaycnt)
//{
//	for(;delaycnt>0;delaycnt--)
//	{
//		__nop();
//	}
//}
