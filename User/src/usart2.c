#include "usart2.h"

/**********************************************************
*函数功能：USART2初始化
*参    数：u32 baud波特率
*返 回 值：None
*备    注：PA2---TX   PA3----RX
***********************************************************/
void USART2_Config(u32 baud)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = baud;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART2, &USART_InitStruct);

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);

	USART_Cmd(USART2, ENABLE);
}


//UAART1发送一个字节
void USART2_SendByte(u8 Byte)
{
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, Byte);
}

//UAART1发送一个字符串
void USART2_SendStr(char* str)
{
	while (*str != '\0')
	{
		USART2_SendByte(*str);
		str++;
	}
}


WiFi_INFO WiFi_Rev = { 0 };
//串口2空闲中断后将数据打印出来
void RECV_USART2(void)
{
	if (WiFi_Rev.r_Flag == 1)
	{
		WiFi_Rev.r_Flag = 0;
		printf("%s\r\n", WiFi_Rev.r_buf);
		memset(WiFi_Rev.r_buf, 0, sizeof(WiFi_Rev.r_buf));
	}
}

extern volatile u8 WiFi_Flag;

void USART2_IRQHandler(void)
{
	u8 clear = 0;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)//接收中断
	{
		//清除中断标志位
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		WiFi_Rev.r_buf[WiFi_Rev.r_len++] = USART_ReceiveData(USART2);
	}
	else if (USART_GetITStatus(USART2, USART_IT_IDLE) == SET)
	{
		clear = USART2->SR;
		clear = USART2->DR;
		(void)clear;
		WiFi_Rev.r_Flag = 1;//接收完成标志
		if (WiFi_Flag == 1)
		{
			WiFi_Rev.r_buf[WiFi_Rev.r_len] = '\0';
			WiFi_Rev.r_len = 0;
		}
	}
}



