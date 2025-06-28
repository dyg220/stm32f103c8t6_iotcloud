#ifndef _RGB_H
#define _RGB_H
#include "stm32f10x.h"

#define RGB_CLK_H	GPIO_SetBits(GPIOB, GPIO_Pin_8) //PB8
#define RGB_CLK_L	GPIO_ResetBits(GPIOB, GPIO_Pin_8) //PB8
#define RGB_DATA_H	GPIO_SetBits(GPIOB, GPIO_Pin_9) //PB9
#define RGB_DATA_L	GPIO_ResetBits(GPIOB, GPIO_Pin_9) //PB9


//DATA-PB9 CLK-PB8
void RGB_Init(void);
u32  Make_Color(u8 R, u8 G, u8 B);
void Send_Word(u32 color);
void RGB_Control(u8 R, u8 G, u8 B);

#endif // !_RGB_H
