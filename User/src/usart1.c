#include "usart1.h"
#include <stdio.h>

void USART1_Config(u32 baud)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // enable clock for GPIOA
	GPIO_InitTypeDef GPIO_InitStructure = { 0 };
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;       // PA9 (TX)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // Alternate function push-pull for TX
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;            // PA10 (RX)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // Input floating for RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     // Set speed to 50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_InitTypeDef USART_InitStructure = { 0 };
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
	// USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // Enable RXNE interrupt
}

void USART1_SendByte(u8 data)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); // 等发送寄存器空
	USART_SendData(USART1, data);
}

void USART1_SendStr(u8* str)
{
	while (*str != '\0')
	{
		USART1_SendByte(*str);
		str++;
	}
	USART1_SendByte('\0'); // 发送字符串结束符
}

#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE
{
	int handle;
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x)
{
	x = x;
}
void _ttywrch(int ch)
{
	ch = ch;
}
//重定义fputc函数 
int fputc(int ch, FILE* f)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	USART_SendData(USART1, ch);
	return ch;
}

/**********************printf support end**********************/

#endif

