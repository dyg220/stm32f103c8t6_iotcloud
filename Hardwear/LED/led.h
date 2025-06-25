
#ifndef _LED_H_
#define _LED_H_
#include "stm32f10x.h"

#define LED1_ON  GPIO_SetBits(GPIOB, GPIO_Pin_11); // Set pin high
#define LED1_OFF  GPIO_ResetBits(GPIOB, GPIO_Pin_11); // Set pin low

#define LED2_ON  GPIO_SetBits(GPIOB, GPIO_Pin_14); // Set pin high
#define LED2_OFF  GPIO_ResetBits(GPIOB, GPIO_Pin_14); // Set pin low


#define LED3_ON  GPIO_SetBits(GPIOA, GPIO_Pin_11); // Set pin high
#define LED3_OFF  GPIO_ResetBits(GPIOA, GPIO_Pin_11); // Set pin low

void LED_Init(void);
void LEDALL_ON(void);
void LEDALL_OFF(void);

void LED1_FZ(void);
void LED2_FZ(void);
void LED3_FZ(void);


#endif // !_LED_H_

