#include "delay.h"

/*for (uint32_t i = 0; i < 72000 ; i++)
{
	for (uint32_t j = 0; j < 500; j++)
	{
		__asm("nop");
	}
}*/

void delay_us(uint32_t xus)
{
	SysTick->CTRL &= ~(1 << 2);//�ⲿ�ο�ʱ��
	SysTick->LOAD = 9 * xus - 1;//��װ��ֵ
	SysTick->VAL = 0;//�������ֵ
	SysTick->CTRL &= ~(1 << 1);//��ʹ���ж�
	SysTick->CTRL |= (1 << 0);//����������
	while (!(SysTick->CTRL & (1 << 16)));//�ȴ��������
	SysTick->CTRL &= ~(1 << 0);//�رռ�����

}

void delay_ms(uint32_t xms)
{
	while (xms--)
	{
		delay_us(1000);
	}
}
