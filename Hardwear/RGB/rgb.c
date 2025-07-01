#include "rgb.h"
#include "delay.h"
#include <stdio.h>

void RGB_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;		//PB8为时钟，PB9为数据
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;			//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct); //初始化GPIOB

	//空闲状态
	RGB_CLK_H;
	delay_us(1);
}

u32 Make_Color(u8 R, u8 G, u8 B)
{
	u32 color = 0;
	color |= ((u32)0x3 << 30); //11标志位
	color |= (((u8)(~B) >> 6) << 28); //B7' B6' ~B右移6位
	color |= (((u8)(~G) >> 6) << 26); //G7' G6'
	color |= (((u8)(~R) >> 6) << 24); //R7' R6'

	//printf("color:%#x", color);
	color |= (B << 16); //B
	color |= (G << 8); //G
	color |= (R << 0); //R
	//printf("color2:%#x", color);
	return color;
}

void Send_Word(u32 color)
{
	u8 i = 0;
	for (i = 0; i < 32; i++) {
		RGB_CLK_L; //MCU
		delay_us(1); //延时1us
		if (color & (0x80000000) >> i)//高位先发
		{
			RGB_DATA_H;
		}
		else
		{
			RGB_DATA_L;
		}
		delay_us(1);
		RGB_CLK_H; //P9813采集数据
		delay_us(1);
	}
}

void RGB_Control(u8 R, u8 G, u8 B)
{
	u32 color = 0;
	color = Make_Color(R, G, B);
	Send_Word(0x00000000);
	Send_Word(color);
	Send_Word(color);
}
