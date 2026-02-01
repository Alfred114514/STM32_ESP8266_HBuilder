#include "stm32f10x.h"

//网络协议层
#include "onenet.h"

//网络设备
#include "esp8266.h"

//硬件驱动
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "dht11.h"
#include "oled.h"
//C库
#include <string.h>


#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"

void Display_Init(void);
void Hardware_Init(void);
void Refresh_Data(void);
u8 temp,humi;

/*
************************************************************		
************************************************************
*/



int main(void)
{
	
	unsigned short timeCount = 0;	//发送间隔变量
	
	unsigned char *dataPtr = NULL;
	
	Hardware_Init();				//初始化外围硬件
	
	ESP8266_Init();					//初始化ESP8266
//	

//	
	UsartPrintf(USART_DEBUG, "Connect MQTTs Server...\r\n");
	while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
		DelayXms(500);
	UsartPrintf(USART_DEBUG, "Connect MQTTs Server  Success\r\n");
	
	
	while(OneNet_DevLink())			//接入OneNET
		DelayXms(500);
	OneNET_Subscribe();

	
	while(1)
	{
	
		if(++timeCount >= 200)									//发送间隔5s
		{
DHT11_Read_Data(&temp,&humi);
			
//			UsartPrintf(USART_DEBUG, "OneNet_SendData\r\n");
			OneNet_SendData();									//发送数据
			
			timeCount = 0;
			ESP8266_Clear();
		}
		
		dataPtr = ESP8266_GetIPD(0);
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
	Refresh_Data();
	DelayMs(10);
	
	}

}

void Hardware_Init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断控制器分组设置

	Delay_Init();									//systick初始化
	
	Usart1_Init(115200);							//串口1，打印信息用
	
	Usart2_Init(115200);							//串口2，驱动ESP8266用
	
//	IIC_Init();										//软件IIC初始化
	
	Key_Init();
	
	Led_Init();									//蜂鸣器初始化
	
	while(DHT11_Init())
	{
		UsartPrintf(USART_DEBUG,"DHT11 Error \r\n");
		DelayMs(1000);
	}
	
	Display_Init();
	
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	
}


void Display_Init(void)
{
    OLED_Init();
	OLED_Clear();
	//温度数值
	OLED_ShowString(0,0,"Temp:",16);//温度显示
	
	OLED_ShowString(0,3,"Humi:",16);

    OLED_ShowString(0,6,"Lamp:",16);

}
void Refresh_Data(void)
{
	char  buf[3];
sprintf(buf, "%2d", temp);
OLED_ShowString(54,0,buf,16);//温度数值
	
sprintf(buf, "%2d", humi);	
OLED_ShowString(54,3,buf,16);//湿度
	
if(led_info.Led_Status) OLED_ShowString(54,6,"ON",16);
  else OLED_ShowString(54,6,"OFF",16);
}