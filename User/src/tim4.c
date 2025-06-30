#include "tim4.h"

/****************************************************
函数功能：TIM4定时中断函数
参    数：None
返 回 值：None
备    注：定时中断1ms
*****************************************************/
void TIM4_ITConfig(u32 Time)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_InternalClockConfig(TIM4);//选择内部时钟源

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = { 0 };
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分频(不用）
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = Time * 100 - 1;//定时1ms
	TIM_TimeBaseInitStruct.TIM_Prescaler = 720 - 1;//计一个数10us
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);

	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//使能更新中断控制位


	NVIC_InitTypeDef NVIC_InitStruct = { 0 };
	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 6;//抢占优先级  
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;////响应优先级
	NVIC_Init(&NVIC_InitStruct);

	TIM_Cmd(TIM4, ENABLE);//使能定时器
}


void TIM4_IRQHandler(void)
{
	//更新中断
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
		//清除标志位
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		//紧急事件
	}
}