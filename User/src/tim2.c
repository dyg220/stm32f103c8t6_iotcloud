#include "tim2.h"

/****************************************************
�������ܣ�TIM2��ʼ������
��    ����None
�� �� ֵ��None
��    ע��TIM2_CH2
*****************************************************/
void TIM2_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_InternalClockConfig(TIM2);//ѡ���ڲ�ʱ��Դ
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//ʱ�ӷ�Ƶ(���ã�
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1;//   ����10ms
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72-1;//1MHz   ��һ����1us
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	TIM_ICInitTypeDef TIM_ICInitStruct = {0};
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;//ͨ��2
	TIM_ICInitStruct.TIM_ICFilter = 0xF;//�˲�
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;//�½��ش�������
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;//һ�����ش���һ��
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;//IC2��ӦTI2
	TIM_ICInit(TIM2,&TIM_ICInitStruct);
	
	
	NVIC_InitTypeDef NVIC_InitStruct = {0};
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;//�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�0~15
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;//��Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStruct);
	
	//ʹ���жϿ���λ
	TIM_ITConfig(TIM2,TIM_IT_CC2,ENABLE);//�����ж�
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//�����ж�
	
	TIM_Cmd(TIM2,ENABLE);//������ʱ��
}










