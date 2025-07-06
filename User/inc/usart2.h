#ifndef _USART2_H_
#define _USART2_H_
#include "stm32f10x.h"
#include "delay.h"

#define Max_Size 256

typedef struct
{
    u8 data[Max_Size];
    u16 index;
    u8 flag;

} USART2_INFO;


void USART2_Config(u32 Baud);
void USART2_SendByte(u8 Byte);
void USART2_SendStr(u8* str);


#endif // !_USART2_H_
