#ifndef __UART_PROTOCAL_H__
#define __UART_PROTOCAL_H__

#define MAX_BUFF_LEN  255
#define MAX_FRAME_LEN 0xF0




typedef struct 
{
	unsigned char buffer[MAX_BUFF_LEN+1];
	int  r_index;
	int  w_index;
} uart_buffer_t;


#endif

typedef struct 
{
	unsigned char cmd;
	int (*action)(unsigned char src,unsigned char *payload,int len);
} uart_cmd_action_t;

enum UARTCMD
{
	AT_CMD=0x01,
	AT_CMD_RSP,
	RESET_WIFI_CMD=0x04,
	DEVICE_ID_CMD,
	SMARTLINK_CMD,
	ONOFF_CMD=0x20,
	BRIGHTNESS_CMD,
	ERROR_REPORT_CMD,
	ONOFF_REPORT_CMD,
	BRIGHTNESS_REPORT_CMD,
	ONOFF_OWNER_CMD=0x26,
    COLOR_CMD,
    COLORTEMP_CMD,
    COLOR_REPORT_CMD,
    COLORTEMP_REPORT_CMD,
	RESPONSE_CMD=0xF0,
};

#define BLUETOOTH_ID 0x01
#define WIFI_MODULE_ID 0x02
#define CONTROL_MCU_ID 0x03


extern uart_buffer_t g_uart0;



int Get_Uart_bufferlen(uart_buffer_t *Q);
void init_uart_buffer(void);
void Push_data(uart_buffer_t *Q,unsigned char *data,int len);
int Pop_data(uart_buffer_t *Q,int start,unsigned char *buff,int len,int flag);
int MakeUartPacket(unsigned char *packet,int *len,unsigned char src,unsigned char dest,unsigned char cmd,unsigned char *payload,int plen);
int ParaseUartFrame(uart_buffer_t *Q);
void TurnOnOff_Light(bool v_switch);
void SetCoolBrt(u16 LoadValu);
void SetWarmBrt(u16 LoadValu);
void SetWiFiToSmlink(void);
void upload_light_onoff(unsigned char onoff,unsigned char mothod);
void upload_light_brightness(unsigned char brightness,unsigned char method);

