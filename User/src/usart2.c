#include "usart2.h"
#include <stdio.h>
#include <string.h>

void USART2_Config(u32 Baud)
{
	// 开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	// 配置GPIO
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStruct);


	// 开时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	USART_InitTypeDef USART_InitStruct = { 0 };
	USART_InitStruct.USART_BaudRate = Baud;                                      // 波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 硬件流控制
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 // 使能发送器和接收器
	USART_InitStruct.USART_Parity = USART_Parity_No;                             // 无校验位
	USART_InitStruct.USART_StopBits = USART_StopBits_1;                          // 停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;                     // 数据位
	USART_Init(USART2, &USART_InitStruct);

	NVIC_InitTypeDef NVIC_InitStruct = { 0 };
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);

	// 使能接受+空闲中断控制位
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

	USART_Cmd(USART2, ENABLE); // 使能USART2
}

/****************************************************
函数功能：USART1发送一个字节
参    数：发送的数据
返 回 值：None
备    注：等待发送寄存器为空时可发送
*****************************************************/
void USART2_SendByte(u8 Byte)
{
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
		; // 等待发送寄存器为空时可发送
	USART_SendData(USART1, Byte);
}

/****************************************************
函数功能：USART1发送字符串
参    数：发送的数据
返 回 值：None
备    注：检测到空字符时停止发送
*****************************************************/
void USART2_SendStr(u8* str)
{
	while (*str != '\0')
	{
		USART2_SendByte(*str);
		str++;
	}
	USART2_SendByte('\0');
}

USART2_INFO Wifi_Recv = { 0 };

// 中断服务函数
void USART2_IRQHandler(void)
{
	u8 clear = 0;
	// 接受中断
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE); // 清除中断标志位
		if (Wifi_Recv.index < Max_Size - 1)           // 防止溢出
		{
			Wifi_Recv.data[Wifi_Recv.index] = USART_ReceiveData(USART2);
			Wifi_Recv.index++;
		}
	}
	// 空闲中断
	if (USART_GetITStatus(USART2, USART_IT_IDLE) == SET)
	{
		// 清除中断标志位
		clear = USART1->SR;
		clear = USART1->DR;
		(void)clear;
		Wifi_Recv.data[Wifi_Recv.index] = '\0';
		Wifi_Recv.flag = 1;
		Wifi_Recv.index = 0;
	}
}

void RECV_USART2(void)
{
	if (Wifi_Recv.flag == 1)
	{
		printf("%s\r\n", Wifi_Recv.data);
		Wifi_Recv.flag = 0;
		memset(Wifi_Recv.data, 0, sizeof(Wifi_Recv.data));
	}
}

