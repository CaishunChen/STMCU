/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	main.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-01-09
	*
	*	版本： 		V1.0
	*
	*	说明： 		USART1打印
	*
	*	修改记录：	
	************************************************************
	************************************************************
	************************************************************
**/

//单片机头文件
#include "stm32f10x.h"

//硬件驱动
#include "delay.h"
#include "usart.h"



/*
************************************************************
*	函数名称：	Hardware_Init
*
*	函数功能：	硬件初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		初始化单片机功能以及外接设备
************************************************************
*/
void Hardware_Init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//中断控制器分组设置

	Delay_Init();										//Systick初始化，用于普通的延时
	
	Usart1_Init(115200);								//初始化串口1，波特率115200

}

/*
************************************************************
*	函数名称：	main
*
*	函数功能：	USART1打印数据
*
*	入口参数：	无
*
*	返回参数：	0
*
*	说明：		
************************************************************
*/
int main(void)
{

	Hardware_Init();									//硬件初始化
	
	while(1)
	{
	
		UsartPrintf(USART1, "\r\n麒麟座开发板-V2.2\r\n");		//打印
		UsartPrintf(USART1, "欢迎登陆中移物联官方网站：open.iot.10086.cn\r\n");
		
		DelayMs(2500);
	
	}

}
