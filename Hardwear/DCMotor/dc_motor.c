#include "dc_motor.h"
#include "delay.h"

void dcMotor_Config(void)
{
	//开启GPIOC的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //禁用JTAG，启用SWD
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //tim3部分重映射


	//定义结构体
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //开启定时器3的时钟
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1; //预分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInitStruct.TIM_Period = 101 - 1; //10ms 0-100
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //时钟分频，不用
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0; //只用于高级定时器
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct); //初始化定时器3

	TIM_InternalClockConfig(TIM3); //选择内部时钟源

	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; //PWM模式1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //输出使能
	TIM_OCInitStruct.TIM_Pulse = 500; //占空比50%
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; //极性高
	TIM_OC1Init(TIM3, &TIM_OCInitStruct); //通道1初始化

	TIM_Cmd(TIM3, ENABLE); //使能定时器3

	MOTOR_IB_Low;
	MOTOR_IA_Low;

}
void MotorRun1(void)
{
	MOTOR_IB_High;
	delay_ms(400);
	MOTOR_IB_Low;

}
void MotorRun2(void) {
	MOTOR_IB_Low;
	delay_ms(400);
	MOTOR_IA_High;
}
void ClosedcMotor(void) {
	MOTOR_IB_High;
	delay_ms(400);
	MOTOR_IA_High;
}