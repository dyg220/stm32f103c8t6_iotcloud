#ifndef _USART2_H_
#define _USART2_H_

#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "wifi.h"



typedef struct
{
	char r_buf[1024];
	u16 r_len;
	u8 r_Flag;
}WiFi_INFO;


void USART2_Config(u32 baud);
void USART2_SendByte(u8 Byte);
void USART2_SendStr(char* str);
void RECV_USART2(void);


#endif
