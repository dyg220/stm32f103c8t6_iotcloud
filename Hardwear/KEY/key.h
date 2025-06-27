
#ifndef _DELAY_H
#define _DELAY_H
#include "stm32f10x.h"

enum KEY {
	key1_click = 1,
	key2_click,
	key3_click,
	key1_long,
	key2_long,
	key3_long
};


#define Key1_Press GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
#define Key2_Press !(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8))
#define Key3_Press !(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10))

#define Key_Mode     0  //1���������   0����������


void Key_Init(void);
u8 Key_Scan(void);
u8 Key_GetState(void);
void Key_Tick(void);

#endif
