#ifndef _USART1_H_
#define _USART1_H_

#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "string.h"
#include "usart2.h"

#define Max_Size   256

typedef struct{
	u8 data[Max_Size];
	u16 index;
	u8 flag;
}USART1_INFO;



void USART1_Config(u32 Baud);
void USART1_SendByte(u8 Byte);
void USART1_SendStr(u8* str);
void RECV_USART1(void);


#endif
