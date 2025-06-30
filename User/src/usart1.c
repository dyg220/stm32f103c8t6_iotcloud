#include "usart1.h"
#include <stdio.h>

/****************************************************
函数功能：USART1初始化函数
参    数：Baud 波特率
返 回 值：None
备    注：USART1_TX---PA9	复用推挽
		  USART1_RX---PA10  浮空输入
*****************************************************/
void USART1_Config(u32 Baud)
{
	//开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//配置GPIO
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	USART_InitTypeDef USART_InitStruct = { 0 };
	USART_InitStruct.USART_BaudRate = Baud;//波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//使能发送器和接收器
	USART_InitStruct.USART_Parity = USART_Parity_No;//无校验位
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//数据位
	USART_Init(USART1, &USART_InitStruct);

	NVIC_InitTypeDef NVIC_InitStruct = { 0 };
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);

	//使能接受+空闲中断控制位
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

	USART_Cmd(USART1, ENABLE);//使能USART1
}

/****************************************************
函数功能：USART1发送一个字节
参    数：发送的数据
返 回 值：None
备    注：等待发送寄存器为空时可发送
*****************************************************/
void USART1_SendByte(u8 Byte)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//等待发送寄存器为空时可发送
	USART_SendData(USART1, Byte);
}

/****************************************************
函数功能：USART1发送字符串
参    数：发送的数据
返 回 值：None
备    注：检测到空字符时停止发送
*****************************************************/
void USART1_SendStr(u8* str)
{
	while (*str != '\0')
	{
		USART1_SendByte(*str);
		str++;
	}
	USART1_SendByte('\0');
}


USART1_INFO USART1_Recv = { 0 };

//中断服务函数
void USART1_IRQHandler(void)
{
	u8 clear = 0;
	//接受中断
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);//清除中断标志位
		if (USART1_Recv.index < Max_Size - 1)//防止溢出
		{
			USART1_Recv.data[USART1_Recv.index] = USART_ReceiveData(USART1);
			USART1_Recv.index++;
		}
	}
	//空闲中断
	if (USART_GetITStatus(USART1, USART_IT_IDLE) == SET)
	{
		//清除中断标志位
		clear = USART1->SR;
		clear = USART1->DR;
		(void)clear;
		USART1_Recv.data[USART1_Recv.index] = '\0';
		USART1_Recv.flag = 1;
		USART1_Recv.index = 0;
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


/****************************************************
函数功能：printf重定向
参    数：None
返 回 值：None
备    注：
*****************************************************/
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
