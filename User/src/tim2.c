#include "tim2.h"

/****************************************************
函数功能：TIM2初始化函数
参    数：None
返 回 值：None
备    注：TIM2_CH2
*****************************************************/
void TIM2_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_InternalClockConfig(TIM2);//选择内部时钟源

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = { 0 };
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分频(不用）
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1;//   周期10ms
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;//1MHz   计一个数1us
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

	TIM_ICInitTypeDef TIM_ICInitStruct = { 0 };
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;//通道2
	TIM_ICInitStruct.TIM_ICFilter = 0xF;//滤波
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;//下降沿触发捕获
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;//一个边沿触发一次
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;//IC2对应TI2
	TIM_ICInit(TIM2, &TIM_ICInitStruct);


	NVIC_InitTypeDef NVIC_InitStruct = { 0 };
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;//中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级0~15
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;//响应优先级
	NVIC_Init(&NVIC_InitStruct);

	//使能中断控制位
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);//捕获中断
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//更新中断

	TIM_Cmd(TIM2, ENABLE);//开启定时器
}










