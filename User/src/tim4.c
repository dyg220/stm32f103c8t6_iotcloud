#include "tim4.h"

/****************************************************
�������ܣ�TIM4��ʱ�жϺ���
��    ����None
�� �� ֵ��None
��    ע����ʱ�ж�1ms
*****************************************************/
void TIM4_ITConfig(u32 Time)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_InternalClockConfig(TIM4);//ѡ���ڲ�ʱ��Դ

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = { 0 };
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//ʱ�ӷ�Ƶ(���ã�
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = Time * 100 - 1;//��ʱ1ms
	TIM_TimeBaseInitStruct.TIM_Prescaler = 720 - 1;//��һ����10us
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);

	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//ʹ�ܸ����жϿ���λ


	NVIC_InitTypeDef NVIC_InitStruct = { 0 };
	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 6;//��ռ���ȼ�  
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;////��Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStruct);

	TIM_Cmd(TIM4, ENABLE);//ʹ�ܶ�ʱ��
}


void TIM4_IRQHandler(void)
{
	//�����ж�
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
		//�����־λ
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		//�����¼�
	}
}