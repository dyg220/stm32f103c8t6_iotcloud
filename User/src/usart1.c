#include "usart1.h"
#include <stdio.h>

/****************************************************
�������ܣ�USART1��ʼ������
��    ����Baud ������
�� �� ֵ��None
��    ע��USART1_TX---PA9	��������
		  USART1_RX---PA10  ��������
*****************************************************/
void USART1_Config(u32 Baud)
{
	//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//����GPIO
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	USART_InitTypeDef USART_InitStruct = { 0 };
	USART_InitStruct.USART_BaudRate = Baud;//������
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ��������
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//ʹ�ܷ������ͽ�����
	USART_InitStruct.USART_Parity = USART_Parity_No;//��У��λ
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//ֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//����λ
	USART_Init(USART1, &USART_InitStruct);

	NVIC_InitTypeDef NVIC_InitStruct = { 0 };
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);

	//ʹ�ܽ���+�����жϿ���λ
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

	USART_Cmd(USART1, ENABLE);//ʹ��USART1
}

/****************************************************
�������ܣ�USART1����һ���ֽ�
��    �������͵�����
�� �� ֵ��None
��    ע���ȴ����ͼĴ���Ϊ��ʱ�ɷ���
*****************************************************/
void USART1_SendByte(u8 Byte)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//�ȴ����ͼĴ���Ϊ��ʱ�ɷ���
	USART_SendData(USART1, Byte);
}

/****************************************************
�������ܣ�USART1�����ַ���
��    �������͵�����
�� �� ֵ��None
��    ע����⵽���ַ�ʱֹͣ����
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

//�жϷ�����
void USART1_IRQHandler(void)
{
	u8 clear = 0;
	//�����ж�
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);//����жϱ�־λ
		if (USART1_Recv.index < Max_Size - 1)//��ֹ���
		{
			USART1_Recv.data[USART1_Recv.index] = USART_ReceiveData(USART1);
			USART1_Recv.index++;
		}
	}
	//�����ж�
	if (USART_GetITStatus(USART1, USART_IT_IDLE) == SET)
	{
		//����жϱ�־λ
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
�������ܣ�printf�ض���
��    ����None
�� �� ֵ��None
��    ע��
*****************************************************/
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE
{
	int handle;
};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x)
{
	x = x;
}
void _ttywrch(int ch)
{
	ch = ch;
}
//�ض���fputc���� 
int fputc(int ch, FILE* f)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	USART_SendData(USART1, ch);
	return ch;
}

/**********************printf support end**********************/

#endif
