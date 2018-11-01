#include "ht32.h"
#include "ht32_board.h"
#include "mast.h"
#include "uart_protocal.h"

uart_buffer_t g_uart0;

/*********************************************************************************************************
**函数名称:upload_light_onoff
**功能描述:上传开关数据
**输入:无		
**输出:
**全局变量:无
********************************************************************************************************/
void upload_light_onoff(unsigned char onoff,unsigned char mothod)
{
    unsigned char packet[100];
    int i,len;
    unsigned char data[2];
    data[0]= onoff;
    data[1]= mothod;
    MakeUartPacket(&packet[0],&len,CONTROL_MCU_ID,WIFI_MODULE_ID,ONOFF_REPORT_CMD,&data[0],2);

    for(i=0;i<len;i++)
        USART_SendData(UR0_WF_COM_PORT,packet[i]);
}
void upload_light_onoff_toBluetooth(unsigned char onoff,unsigned char mothod)
{
    unsigned char packet[100];
    int i,len;
    unsigned char data[2];
    data[0]= onoff;
    data[1]= mothod;
    MakeUartPacket(&packet[0],&len,CONTROL_MCU_ID,BLUETOOTH_ID,ONOFF_REPORT_CMD,&data[0],2);

    for(i=0;i<len;i++)
        USART_SendData(UR0_WF_COM_PORT,packet[i]);

}
/*********************************************************************************************************
**函数名称:upload_light_brightness
**功能描述:上传亮度数据
**输入:无		
**输出:
**全局变量:无
********************************************************************************************************/
void upload_light_brightness(unsigned char brightness,unsigned char method)
{
    unsigned char packet[100];
    int i,len;
    unsigned char data[2];
    data[0] = brightness;
    data[1] = method;
    MakeUartPacket(&packet[0],&len,CONTROL_MCU_ID,WIFI_MODULE_ID,BRIGHTNESS_REPORT_CMD,&data[0],2);
    
    for(i=0;i<len;i++)
        USART_SendData(UR0_WF_COM_PORT,packet[i]);

}

void upload_light_brightness_toBluetooth(unsigned char brightness,unsigned char method)
{
    unsigned char packet[100];
    int i,len;
    unsigned char data[2];
    data[0] = brightness;
    data[1] = method;
    MakeUartPacket(&packet[0],&len,CONTROL_MCU_ID,BLUETOOTH_ID,BRIGHTNESS_REPORT_CMD,&data[0],2);
    
    for(i=0;i<len;i++)
        USART_SendData(UR0_WF_COM_PORT,packet[i]);

}
/*********************************************************************************************************
**函数名称:Onoff_action
**功能描述:开关
**输入:无		
**输出:
**全局变量:无
********************************************************************************************************/
int Onoff_action(unsigned char src,unsigned char *payload,int len)
{
	unsigned char v_switch;
	if( payload == NULL ) return 0;
	
	v_switch = *payload;
	
	TurnOnOff_Light((bool)v_switch);
	
	upload_light_onoff(v_switch,0x01);
	if(src == BLUETOOTH_ID)
	{
		upload_light_onoff_toBluetooth(v_switch,0x01);
	}			
	return 1;
}
/*********************************************************************************************************
**函数名称:UpLoadColorTemp
**功能描述:上传亮度数据
**输入:无		
**输出:
**全局变量:无
********************************************************************************************************/
void UpLoadColorTemp(unsigned char ColorTemp,unsigned char method)
{
    unsigned char packet[100];
    int i,len;
    unsigned char data[2];
    data[0] = ColorTemp;
    data[1] = method;
    MakeUartPacket(&packet[0],&len,CONTROL_MCU_ID,WIFI_MODULE_ID,COLORTEMP_REPORT_CMD,&data[0],2);
    
    for(i=0;i<len;i++)
        USART_SendData(UR0_WF_COM_PORT,packet[i]);

}
/*********************************************************************************************************
**函数名称:ColorTempAct
**功能描述:色温设置
**输入:无		
**输出:
**全局变量:无
********************************************************************************************************/
int ColorTempAct(unsigned char src,unsigned char *payload,int len)
{
	unsigned char ColorTemp;
	if( payload == NULL ) return 0;	
	ColorTemp = *payload;	
	SetColorWarm(ColorTemp);                   //设置光亮度	
	UpLoadColorTemp(ColorTemp,0x01); //上传光亮度
//	if(src == BLUETOOTH_ID)
//	{		
//		upload_light_brightness_toBluetooth(ColorTemp,0x01);//upload status
//	}	
	return 1;
}
/*********************************************************************************************************
**函数名称:UpLoadColor
**功能描述:上传亮度数据
**输入:无		
**输出:
**全局变量:无
********************************************************************************************************/
void UpLoadColor(unsigned char Color,unsigned char method)
{
    unsigned char packet[100];
    int i,len;
    unsigned char data[2];
    data[0] = Color;
    data[1] = method;
    MakeUartPacket(&packet[0],&len,CONTROL_MCU_ID,WIFI_MODULE_ID,COLOR_REPORT_CMD,&data[0],2);
    
    for(i=0;i<len;i++)
        USART_SendData(UR0_WF_COM_PORT,packet[i]);

}
/*********************************************************************************************************
**函数名称:ColorAct
**功能描述:亮度设置
**输入:无		
**输出:
**全局变量:无
********************************************************************************************************/
int ColorAct(unsigned char src,unsigned char *payload,int len)
{
	unsigned char Color;
	if( payload == NULL ) return 0;	
	Color = *payload;	
	SetColor(Color);                   //设置光亮度	
	UpLoadColor(Color,0x01); //上传光亮度
//	if(src == BLUETOOTH_ID)
//	{		
//		upload_light_brightness_toBluetooth(Color,0x01);//upload status
//	}	
	return 1;
}
/*********************************************************************************************************
**函数名称:Brightness_action
**功能描述:亮度设置
**输入:无		
**输出:
**全局变量:无
********************************************************************************************************/
int Brightness_action(unsigned char src,unsigned char *payload,int len)
{
	unsigned char v_brightness;
	if( payload == NULL ) return 0;	
	v_brightness = *payload;	
	SetBrightness(v_brightness);                   //设置光亮度	
	upload_light_brightness(v_brightness,0x01); //上传光亮度
	if(src == BLUETOOTH_ID)
	{		
		upload_light_brightness_toBluetooth(v_brightness,0x01);//upload status
	}	
	return 1;
}
int AT_CMD_rsp_action(unsigned char src,unsigned char *payload,int len)
{
    //Do something here
    int i;
    for(i=0;i<len;i++)
        USART_SendData(UR0_WF_COM_PORT,*payload++);	
    return 1;
}
/*********************************************************************************************************
**函数名称:SetWiFiToSmlink
**功能描述:wifi连接路由器
**输入:无		
**输出:
**全局变量:无
********************************************************************************************************/
void SetWiFiToSmlink(void)
{
    unsigned char packet[100];
    int i,len;
    unsigned char smlink[1];
    smlink[0]= 0x01;
    MakeUartPacket(&packet[0],&len,CONTROL_MCU_ID,WIFI_MODULE_ID,SMARTLINK_CMD,&smlink[0],1);
    for(i=0;i<len;i++)
        USART_SendData(UR0_WF_COM_PORT,packet[i]);
//    unsigned char packet[100];    //想使用AT局域网升级固件发现不行
//    int i,len;    
//    unsigned char smlink[] = "AT+OTA \r\n";
//    MakeUartPacket(&packet[0],&len,CONTROL_MCU_ID,WIFI_MODULE_ID,AT_CMD,&smlink[0],sizeof(smlink));
//    for(i=0;i<len;i++)
//        USART_SendData(UR0_WF_COM_PORT,packet[i]);
}

//命令执行结构体数组：命令，指向函数的指针
uart_cmd_action_t uart0_action[]=
{
	{ONOFF_CMD,Onoff_action},
	{BRIGHTNESS_CMD,Brightness_action},
	{COLOR_CMD,ColorAct},
	{COLORTEMP_CMD,ColorTempAct},    
	{AT_CMD_RSP,AT_CMD_rsp_action},
	{0xFF,NULL}
};  

/*********************************************************************************************************
**函数名称:MakeUartPacket
**功能描述:串口数据打包
**输入:无		
**输出:
**全局变量:无
********************************************************************************************************/
int MakeUartPacket(unsigned char *packet,int *len,unsigned char src,unsigned char dest,unsigned char cmd,unsigned char *payload,int plen)
{
	int i;
	unsigned char checksum=0;	
	if(packet == NULL) return 0;
	//header
	*packet++ = 0xf1;
	*packet++ = 0xf2;
	*packet++ = plen + 8;  //length

	*packet++ = src;
	*packet++ = dest;
	*packet++ = cmd;
	checksum = src + dest + cmd + plen + 8;	
	for(i=0;i<plen;i++)
	{
		*packet++ = *payload;
		checksum += *payload++;
	}
	*packet++=checksum;
	*packet = 0xF9;

	*len = plen + 8;
	return 1;
	
}
/*********************************************************************************************************
**函数名称:Frame_process
**功能描述:帧命令分析
**输入:无		
**输出:
**全局变量:无
********************************************************************************************************/
void Frame_process(uart_buffer_t *Q,unsigned char *data,int len)
{
    unsigned char cmd,i,src;
    src = data[3];
    if(data[4]== CONTROL_MCU_ID) //to MCU
    {
        cmd = data[5];
        if(Q == &g_uart0)
        {
            for(i=0;;i++)
            {
                if(uart0_action[i].cmd == 0xFF)  
                    return;
                if(uart0_action[i].cmd == cmd )
                {
                    if(uart0_action[i].action)
                        uart0_action[i].action(src,&data[6],len-8);
                    return;
                }
            }
        }
    }
}
/*********************************************************************************************************
**函数名称:ParaseUartFrame
**功能描述:串口数据分析，维护数据队列
**输入:无		
**输出:
**全局变量:无
********************************************************************************************************/

int ParaseUartFrame(uart_buffer_t *Q)
{
	int i;
	unsigned char frame_len=0;
	int header=0,start;
	unsigned char checksum=0;
	unsigned char data[MAX_FRAME_LEN];
	if(Q == NULL) return 0;
	if(Q->r_index == Q->w_index ) return 0;

	i=Q->r_index;
	if(i == Q->w_index)
	{
		return 0;
	}
	for(;;)
	{
		if(i == MAX_BUFF_LEN )
		{
            if(Q->buffer[i]==0xF1 && Q->buffer[0]==0xF2)
            {
                start = i;
                header = 1;
                i=0;
            }
		}	
		else
		{
			if(Q->buffer[i]==0xF1 && Q->buffer[i+1]==0xF2)
			{
				start = i;
				i++;
				header = 1;
			}
		}
		if( i== Q->w_index)
		{
			return 0;
		}		
		i++;
		if(i> MAX_BUFF_LEN ) i=0;

		if( i== Q->w_index)
		{
			return 0;
		}
		if( header == 1)
		{
			frame_len = Q->buffer[i];

			if(frame_len >= 0xF0 || frame_len < 0x08) 
			{
				header = 0;
				continue;
			}

			if(frame_len > Get_Uart_bufferlen(Q))
			{
				return 0;
			}
			break;
		}
	}
	//
	if (Pop_data(Q, start,data,frame_len,1)==0 ) return 0;
	if(data[frame_len -1] != 0xF9) // ??
	{	
		Pop_data(Q, start,data,2,0);	//????????
		return 1;
	}
	for(i=2;i<frame_len-2;i++)
	{	
		checksum += data[i];
	}
	if( checksum == data[i]) //checksum ok
	{
		if(Pop_data(Q, start,data,frame_len,0))
		{		
            LED_GREEN_ON;
			Frame_process(Q,data,frame_len);//帧数据处理	
			return 1;
		}
	}
	else
	{
		Pop_data(Q, start,data,frame_len,0);//弹出数据
		return 1;
	}

	return 0;
}
/*********************************************************************************************************
**函数名称:init_uart_buffer
**功能描述:初始化数据缓冲区
**输入:无		
**输出:
**全局变量:无
********************************************************************************************************/
void init_uart_buffer(void)
{
	//memset(g_uart0.buffer,0,sizeof(g_uart0.buffer));
	int i;
	
	for(i=0;i<=MAX_BUFF_LEN;i++)
		g_uart0.buffer[i]=0;
	
	g_uart0.r_index=0;
	g_uart0.w_index=0;

}
/*********************************************************************************************************
**函数名称:Get_Uart_bufferlen
**功能描述:获取数据缓冲区长度
**输入:无		
**输出:
**全局变量:无
********************************************************************************************************/
int Get_Uart_bufferlen(uart_buffer_t *Q)
{
	int len;
	if(Q==NULL) return 0;
	if(Q->r_index > Q->w_index)
	{
		len = MAX_BUFF_LEN - Q->r_index + 1 + Q->w_index;
	}
	else
	{
		len = Q->w_index -Q->r_index;
	}
	return len;
}


/*********************************************************************************************************
**函数名称:Push_data
**功能描述:串口队列压入数据
**输入:无		
**输出:
**全局变量:无
********************************************************************************************************/
void Push_data(uart_buffer_t *Q,unsigned char *data,int len)
{
	int i;
	if(Q == NULL) return;
	for(i=0;i<len;i++)
	{	
		if(Q->w_index >= MAX_BUFF_LEN)
		{
			Q->buffer[Q->w_index]=*data++;
			Q->w_index = 0;
		}
		else
		{
			Q->buffer[Q->w_index++]=*data++;
		}
		if(Q->w_index == Q->r_index) 
		{
			Q->r_index +=1;//r=w Q is NULL r<>w Q has data
			if(Q->r_index>MAX_BUFF_LEN)
				Q->r_index = 0;
		}
	}
}

/*********************************************************************************************************
**函数名称:Pop_data
**功能描述:串口队列弹出数据
**输入:无		
**输出:
**全局变量:无
********************************************************************************************************/
int Pop_data(uart_buffer_t *Q,int start,unsigned char *buff,int len,int flag)
{
	int i,v_r_index,v_len;

	if(Q == NULL ) return 0;
	if(buff ==NULL) return 0;

	if(Q->w_index == Q->r_index ) return 0;
	if(Q->w_index > Q->r_index)
	{
		if(start< Q->r_index || start >= Q->w_index) return 0;
		v_len = Q->w_index- start;
	}
	else
	{
		if(start >= Q->w_index && start <Q->r_index) return 0;
		if( start > Q->w_index )
			v_len = MAX_BUFF_LEN - start + 1 + Q->w_index;
		else
			v_len = Q->w_index -start;
	}
	
	if(v_len < len ) return 0;
	
	v_r_index = start;
	
	for(i=0;i<len;i++)
	{
		*buff++ = Q->buffer[v_r_index++];
		if(v_r_index > MAX_BUFF_LEN)
		{
			v_r_index = 0;
			
		}
	}
	if(flag==0)	Q->r_index = v_r_index;
	return 1;

}
