#include "dc_motor.h"
#include "delay.h"

void dcMotor_Config(void)
{
	//����GPIOC��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //����JTAG������SWD
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //tim3������ӳ��


	//����ṹ��
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //������ʱ��3��ʱ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1; //Ԥ��Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period = 101 - 1; //10ms 0-100
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //ʱ�ӷ�Ƶ������
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0; //ֻ���ڸ߼���ʱ��
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct); //��ʼ����ʱ��3

	TIM_InternalClockConfig(TIM3); //ѡ���ڲ�ʱ��Դ

	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; //PWMģʽ1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //���ʹ��
	TIM_OCInitStruct.TIM_Pulse = 500; //ռ�ձ�50%
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; //���Ը�
	TIM_OC1Init(TIM3, &TIM_OCInitStruct); //ͨ��1��ʼ��

	TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��3

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