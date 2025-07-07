#ifndef _WIFI_H_
#define _WIFI_H_

#include "stm32f10x.h" // Device header
#include "usart2.h"

void WiFi_Init(void);
void WiFi_SendAT(char *AT, char *respond, u32 TimeOut);


#endif
