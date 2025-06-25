#include "key.h"
#include "delay.h"
#include "led.h"

u8 Key_Num = 0;

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	// Configure GPIOB Pin 11 as output push-pull
	GPIO_InitTypeDef GPIO_InitStructure = {
		.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8, // key1: PA0  key2: PA8  key3: PB10
		.GPIO_Mode = GPIO_Mode_IN_FLOATING  // Output mode£∫ ∏°ø’ ‰»Î
		//.GPIO_Speed = GPIO_Speed_2MHz     // Œﬁ–Ë≈‰÷√
	};

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
u8 Key_Scan(void)
{
#if  Key_Mode
	if (Key_Num)
	{
		u8 Temp = 0;
		Temp = Key_Num;
		Key_Num = 0;
		return Temp;
	}
	return 0;
#else
	u16 time_cnt = 0;
	u8 key_num = 0;
	if (Key1_Press)
	{
		delay_ms(20);
		if (Key1_Press)
		{
			while (Key1_Press)
			{
				time_cnt++;
				delay_ms(1);
			}
			if (time_cnt < 500)
				key_num = key1_click;
			else
				key_num = key1_long;
		}
	}
	if (Key2_Press)
	{
		delay_ms(20);
		if (Key2_Press)
		{
			while (Key2_Press)
			{
				time_cnt++;
				delay_ms(1);
			}
			if (time_cnt < 500)
				key_num = key2_click;
			else
				key_num = key2_long;
		}
	}
	if (Key3_Press)
	{
		delay_ms(20);
		if (Key3_Press)
		{
			while (Key3_Press)
			{
				time_cnt++;
				delay_ms(1);
			}
			if (time_cnt < 500)
				key_num = key3_click;
			else
				key_num = key3_long;
		}
	}
	return key_num;
#endif
}
u8 Key_GetState(void)
{
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_SET)
	{
		return key1_click;
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == Bit_SET)
	{
		return key2_click;
	}
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == Bit_RESET)
	{
		return key3_click;
	}
	return 0;
}
void Key_Tick(void)
{
	static u32 Count = 0;
	static u8 CurrState = 0, PrevState = 0;

	Count++;
	if (Count >= 20)
	{
		Count = 0;
		PrevState = CurrState;
		CurrState = Key_GetState();
		if (CurrState == 0 && PrevState != 0)
		{
			Key_Num = PrevState;
		}
	}
}
