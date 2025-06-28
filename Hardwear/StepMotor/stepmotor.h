#ifndef _STEPMOTOR_H_
#define _STEPMOTOR_H_

#include "stm32f10x.h"                  // Device header
#include "delay.h"

#define PA7_H	GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define PA7_L	GPIO_ResetBits(GPIOA,GPIO_Pin_7)

#define PA6_H	GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define PA6_L	GPIO_ResetBits(GPIOA,GPIO_Pin_6)

#define PA5_H	GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define PA5_L	GPIO_ResetBits(GPIOA,GPIO_Pin_5)

#define PA4_H	GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define PA4_L	GPIO_ResetBits(GPIOA,GPIO_Pin_4)

void StepMotor_Init(void);
void StepMotor_Control(int spin, u8 dri);

#endif
