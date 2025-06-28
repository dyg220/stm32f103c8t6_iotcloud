#include "stepmotor.h"

/****************************************************
�������ܣ����������ʼ������
��    ����None
�� �� ֵ��None
��    ע��PA7--A  PA6--A'  PA5--B  PA4--B'
*****************************************************/
void StepMotor_Init(void)
{
	//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//����GPIO
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	PA7_L;
	PA6_L;
	PA5_L;
	PA4_L;
}


/****************************************************
�������ܣ�����������ƺ���
��    ����Ȧ�� 0~200    ����1 ����2
�� �� ֵ��None
��    ע��
*****************************************************/
void StepMotor_Control(int spin, u8 dri)
{
	u16 i = 0;
	static u8 num = 1;
	for (i = 0; i < 4 * spin; i++)
	{
		if (dri == 1)
		{
			switch (num)
			{
			case 1:PA7_H; PA6_L; PA5_L; PA4_H; break;
			case 2:PA7_L; PA6_H; PA5_L; PA4_H; break;
			case 3:PA7_L; PA6_H; PA5_H; PA4_L; break;
			case 4:PA7_H; PA6_L; PA5_H; PA4_L; break;
			}
		}
		if (dri == 2)
		{
			switch (num)
			{
			case 4:PA7_H; PA6_L; PA5_L; PA4_H; break;
			case 3:PA7_L; PA6_H; PA5_L; PA4_H; break;
			case 2:PA7_L; PA6_H; PA5_H; PA4_L; break;
			case 1:PA7_H; PA6_L; PA5_H; PA4_L; break;
			}
		}
		delay_ms(1);
		num++;
		if (num > 4)num = 1;
	}
	PA7_L; PA6_L; PA5_L; PA4_L;
}



