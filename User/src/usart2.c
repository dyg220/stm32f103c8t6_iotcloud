#include "usart2.h"
#include <stdio.h>
#include <string.h>

void USART2_Config(u32 Baud)
{
	// ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	// ����GPIO
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; // ��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; // ��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStruct);


	// ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	USART_InitTypeDef USART_InitStruct = { 0 };
	USART_InitStruct.USART_BaudRate = Baud;                                      // ������
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // Ӳ��������
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 // ʹ�ܷ������ͽ�����
	USART_InitStruct.USART_Parity = USART_Parity_No;                             // ��У��λ
	USART_InitStruct.USART_StopBits = USART_StopBits_1;                          // ֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;                     // ����λ
	USART_Init(USART2, &USART_InitStruct);

	NVIC_InitTypeDef NVIC_InitStruct = { 0 };
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);

	// ʹ�ܽ���+�����жϿ���λ
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

	USART_Cmd(USART2, ENABLE); // ʹ��USART2
}

/****************************************************
�������ܣ�USART1����һ���ֽ�
��    �������͵�����
�� �� ֵ��None
��    ע���ȴ����ͼĴ���Ϊ��ʱ�ɷ���
*****************************************************/
void USART2_SendByte(u8 Byte)
{
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
		; // �ȴ����ͼĴ���Ϊ��ʱ�ɷ���
	USART_SendData(USART1, Byte);
}

/****************************************************
�������ܣ�USART1�����ַ���
��    �������͵�����
�� �� ֵ��None
��    ע����⵽���ַ�ʱֹͣ����
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

// �жϷ�����
void USART2_IRQHandler(void)
{
	u8 clear = 0;
	// �����ж�
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE); // ����жϱ�־λ
		if (Wifi_Recv.index < Max_Size - 1)           // ��ֹ���
		{
			Wifi_Recv.data[Wifi_Recv.index] = USART_ReceiveData(USART2);
			Wifi_Recv.index++;
		}
	}
	// �����ж�
	if (USART_GetITStatus(USART2, USART_IT_IDLE) == SET)
	{
		// ����жϱ�־λ
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

