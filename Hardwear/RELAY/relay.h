#ifndef __RELAY_H
#include "stm32f10x.h"

#define RELAY_ON   GPIO_SetBits(GPIOB, GPIO_Pin_1)		// Set pin high to turn on relay
#define RELAY_OFF  GPIO_ResetBits(GPIOB, GPIO_Pin_1)   // Set pin high to turn off relay

void Relay_Init(void);

#endif // __RELAY_H

