

//单片机头文件
#include "stm32f10x.h"

//硬件驱动
#include "key.h"
#include "delay.h"
#include "led.h"
void Key_Init(void)
{

	GPIO_InitTypeDef gpio_initstruct;//引脚初始化
	EXTI_InitTypeDef exti_initstruct;
	NVIC_InitTypeDef nvic_initstruct;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//打开GPIOB的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//开启中断的时钟
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	//禁止JTAG功能
	
	
	gpio_initstruct.GPIO_Mode = GPIO_Mode_IPU;				//设置为输出
	gpio_initstruct.GPIO_Pin = GPIO_Pin_1;						//将初始化的Pin脚
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;				//可承载的最大频率	
	GPIO_Init(GPIOA, &gpio_initstruct);							//初始化GPIO
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);				//按键中断

	
	exti_initstruct.EXTI_Line = EXTI_Line1;
	exti_initstruct.EXTI_LineCmd =ENABLE;
	exti_initstruct.EXTI_Mode =EXTI_Mode_Interrupt;
	exti_initstruct.EXTI_Trigger =EXTI_Trigger_Falling;
	EXTI_Init(& exti_initstruct);
	
	nvic_initstruct.NVIC_IRQChannel=EXTI1_IRQn;
	nvic_initstruct.NVIC_IRQChannelCmd =ENABLE;
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority =2;
	nvic_initstruct.NVIC_IRQChannelSubPriority =2;
	NVIC_Init(&nvic_initstruct);
	//初始化完成
	
}
//中断函数
void EXTI1_IRQHandler(void)
{
DelayXms(10);//延时消抖10毫秒
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)

	{
	if(led_info.Led_Status == LED_ON) Led_Set(LED_OFF);
	else Led_Set(LED_ON);
	}
	//清除中断标志位
EXTI_ClearITPendingBit(EXTI_Line1);


}






