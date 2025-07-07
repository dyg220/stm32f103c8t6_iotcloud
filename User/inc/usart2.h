#ifndef _USART2_H_
#define _USART2_H_

#include "stm32f10x.h" // Device header
#include "wifi.h"
#include <stdio.h>

typedef struct
{
    u8 r_buf[2048];
    u16 r_len;
    u8 r_Flag;

}WiFi_INFO;

extern WiFi_INFO WiFi_Rev;

void USART2_Config(u32 baud);
void USART2_SendByte(u8 Byte);
void USART2_SendStr(char *str);

#endif
