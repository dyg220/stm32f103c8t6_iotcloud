#include "led.h"

void LED_Init(void)
{
	// Enable the GPIOB clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
	// Configure GPIOB Pin 11 as output push-pull
	GPIO_InitTypeDef GPIO_InitStructure = {
		.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_14,       	// Pin 11
		.GPIO_Mode = GPIO_Mode_Out_PP, // Output mode
		.GPIO_Speed = GPIO_Speed_2MHz  // Speed 50MHz
	};

	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	LEDALL_OFF();
}

//全开
void LEDALL_ON()
{
	LED1_ON;
	LED2_ON;
	LED3_ON;
}

//全关
void LEDALL_OFF()
{
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
}

//LED1 翻转
void LED1_FZ()
{
	if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_11) == Bit_SET) //获取LED1状态
	{
		LED1_OFF;
	}
	else
	{
		LED1_ON;
	}
}

void LED2_FZ()
{
	if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_14) == Bit_SET) //获取LED1状态
	{
		LED2_OFF;
	}
	else
	{
		LED2_ON;
	}
}

void LED3_FZ()
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_11) == Bit_SET) //获取LED1状态
	{
		LED3_OFF;
	}
	else
	{
		LED3_ON;
	}
}
