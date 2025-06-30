#ifndef _DHT11_H_
#define _DHT11_H_

#include "stm32f10x.h"                  // Device header
#include "delay.h"

#define DHT11_DATA_H	GPIO_SetBits(GPIOB,GPIO_Pin_3)
#define DHT11_DATA_L	GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define DHT11_DATA_R	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)

void DHT11_Init(void);
void DHT11_Start(void);
u8 DHT11_Respond(void);
u8 DHT11_GetTempHumi(float* temp, u8* humi);


#endif
