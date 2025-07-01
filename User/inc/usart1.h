
#ifndef _USART1_H
#define _USART1_H
#include "stm32f10x.h" // Device header

#define Max_Size 256

typedef struct
{
    u8 data[Max_Size];
    u16 index;
    u8 flag;

} USART1_INFO;

void USART1_Config(u32 baud);
void USART1_SendByte(u8 data);
void USART1_SendStr(u8 *str);
void RECV_USART1(void);

#endif // !_USART1_H
