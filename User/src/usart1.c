#include "usart1.h"
#include <stdio.h>
#include <string.h>


USART1_INFO USART1_Recv = { 0 };

void USART1_Config(u32 baud)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // enable clock for GPIOA
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // enable clock for AFIO

	GPIO_InitTypeDef GPIO_InitStructure = { 0 };
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;       // PA9 (TX) 复用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;            // PA10 (RX) 浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
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


	NVIC_InitTypeDef NVIC_InitStructure = { 0 };
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; // USART1 interrupt channel
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // Preemption priority
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // Subpriority
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // Enable the USART1 interrupt
	NVIC_Init(&NVIC_InitStructure); // Initialize NVIC with the configuration

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //接收中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); // 空闲中断

	USART_Cmd(USART1, ENABLE);

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


void USART1_IRQHandler(void)  //中断服务函数
{
	u8 clear = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) //接收中断
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE); //清除中断标志
		//u8 data = USART_ReceiveData(USART1); //读取接收到的数据
		//printf("%c", data);  //不建议在中断中使用printf，因为printf可能会阻塞，导致中断响应延迟
		if (USART1_Recv.index < Max_Size - 1) // 防止溢出
		{
			USART1_Recv.data[USART1_Recv.index] = USART_ReceiveData(USART1); //;将数据读出还能清除标志位
			USART1_Recv.index++;
		}
	}
	if (USART_GetITStatus(USART1, USART_IT_IDLE) == SET) //空闲中断
	{
		USART_ClearITPendingBit(USART1, USART_IT_IDLE); //清除中断标志
		//clear = USART_ReceiveData(USART1); //读取接收到的数据，清除中断标志
		clear = USART1->SR; // 读取状态寄存器，清除中断标志		
		clear = USART1->DR; // 读取数据寄存器，清除中断标志
		(void)clear; // 避免编译器警告
		USART1_Recv.data[USART1_Recv.index] = '\0';
		USART1_Recv.flag = 1;
		USART1_Recv.index = 0;
		//printf("------IDLE------\r\n");
	}
}

void RECV_USART1(void)
{
	if (USART1_Recv.flag == 1)
	{
		printf("%s\r\n", USART1_Recv.data);
		USART1_Recv.flag = 0;
		memset(USART1_Recv.data, 0, sizeof(USART1_Recv.data));
	}
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

