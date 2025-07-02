#ifndef _OLED_H
#define _OLED_H
#include "stm32f10x.h"
#include "delay.h"

#define OLED_CS_H  GPIO_SetBits(GPIOB, GPIO_Pin_12) // OLEDƬѡ
#define OLED_CS_L  GPIO_ResetBits(GPIOB, GPIO_Pin_12) // OLEDƬѡ

#define OLED_RES_H GPIO_SetBits(GPIOB, GPIO_Pin_6) // OLED��λ
#define OLED_RES_L GPIO_ResetBits(GPIOB, GPIO_Pin_6) // OLED��λ

#define OLED_DC_H  GPIO_SetBits(GPIOB, GPIO_Pin_7) // OLED����/����ѡ��
#define OLED_DC_L  GPIO_ResetBits(GPIOB, GPIO_Pin_7) // OLED����/����ѡ��


void OLED_Pin_Init(void);
void OLED_WriteCommand(u8 Command);
void OLED_WriteData(u8 Data);
void OLED_Init(void);
void OLED_setCursor(u8 X, u8 Page);
void OLED_Clear(void);



#endif // !_OLED_H


