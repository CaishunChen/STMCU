#include "common.h"

//////���е�����Ӳ������

#define  TIM_PSC     479   ////100KHZ 

static uint32 fac_us, fac_ms;

void Delay_Init(void)

{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//systickʱ��= HCLK/8 
	fac_us = SystemCoreClock / 8000000;
	fac_ms = fac_us * 1000;
}

void delay_us(uint16 us)
{
	uint32_t temp;

	SysTick->LOAD = us * fac_us;                  //ʱ�����
	SysTick->VAL = 0x00;                        //���������

	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//��systick��ʱ������ʼ����ʱ

	do
	{
		temp = SysTick->CTRL;
	} while ((temp & 0x01) && !(temp&(1 << 16)));

	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//�ر�systick��ʱ��
	SysTick->VAL = 0x00;//���������
}

void delay_ms(uint16 ms)
{
	uint32_t temp;

	SysTick->LOAD = ms * fac_ms;//ʱ�����
	SysTick->VAL = 0x00;        //���������
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;///��systick��ʱ������ʼ����ʱ

	do
	{
		temp = SysTick->CTRL;
	} while ((temp & 0x01) && !(temp&(1 << 16)));

	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//�ر�systick��ʱ��
	SysTick->VAL = 0x00;       //���������  
}

void uart1_sendchar(uint8 ch)
{
	USART1->TDR = ch;
	while ((USART1->ISR & 0x40) == 0);
}

void uart1_sendbuff(uint8 * buff, uint16 size)
{
	while (size--)
		uart1_sendchar(*buff++);
}

void uart2_sendchar(uint8 ch)
{
	USART2->TDR = ch;
	while ((USART2->ISR & 0x40) == 0);
}

void uart2_sendbuff(uint8 * buff, uint16 size)
{
	while (size--)
		uart2_sendchar(*buff++);
}