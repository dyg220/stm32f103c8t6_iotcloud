#include "Infrared.h"

/****************************************************
�������ܣ������ʼ������
��    ����None
�� �� ֵ��None
��    ע��DATA---PA1----TIM2_CH2
*****************************************************/
void Infrared_Init(void)
{
	//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//����GPIO
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	TIM2_Config();//��ʱ������
}

INFRARED_INFO Infrared_info_t = { 0 };

//TIM2�жϷ�����
void TIM2_IRQHandler(void)
{
	static u8 i = 0;
	static u8 StartFlag = 0;
	//�����ж�
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//����жϱ�־λ
		if (StartFlag == 1)//�ɼ���ʼ
		{
			//�ɼ������У�û�б���ʱ�����10ms�����Ե��ɼ���ʼ��
			//���뵽�����ж�˵���ɼ����
			StartFlag = 0;
			Infrared_info_t.EdgeCnt = i;//������
			i = 0;
			if (Infrared_info_t.EdgeCnt > 10) //���˸����ź�
				Infrared_info_t.CaptureOver = 1;//�������
		}
	}
	//ͨ��2�Ĳ����ж�
	else if (TIM_GetITStatus(TIM2, TIM_IT_CC2) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);//����жϱ�־λ
		if (TIM2->CCER & (1 << 5))//�½��ش�������
		{
			TIM2->CCER &= ~(1 << 5);//�ĳ������ش���
			if (StartFlag == 0)//��һ�β������
			{
				StartFlag = 1;//�ɼ���ʼ��־
				TIM2->CNT = 0;//����������
			}
			else
			{
				Infrared_info_t.TimeCnt[i++] = TIM_GetCapture2(TIM2);//����ߵ�ƽʱ��
				TIM2->CNT = 0;//����������
			}
		}
		else//�����ش�������
		{
			TIM2->CCER |= (1 << 5);//�ĳ��½��ش���
			Infrared_info_t.TimeCnt[i++] = TIM_GetCapture2(TIM2);
			TIM2->CNT = 0;
		}
	}
}

/****************************************************
�������ܣ���������NECλ
��    �������� type 1  67��
�� �� ֵ��0�ɹ�   1ʧ��
��    ע���߼�1��560us��+1680us��
		  �߼�0��560us��+560us��
*****************************************************/
u8 Infrared_AnalysisNECBit(u8 type)
{
	u8 i = 0, j = 0;
	u8 EdgeCnt = 0;
	switch (type)
	{
	case 1:EdgeCnt = 67; break;//���������
	}
	if (Infrared_info_t.EdgeCnt != EdgeCnt)
		return 1;
	for (i = 3; i < EdgeCnt; i += 2)
	{
		if (Infrared_info_t.TimeCnt[i] > 1380 && Infrared_info_t.TimeCnt[i] < 1980)//1  ---- 1680us
			Infrared_info_t.NECBit[j++] = 1;//�����1380~1980Ϊ�߼�1
		else
			Infrared_info_t.NECBit[j++] = 0;//����Ϊ�߼�0
		printf("%d ", Infrared_info_t.NECBit[j - 1]);
	}
	printf("\r\n");
	return 0;
}


/****************************************************
�������ܣ���������NEC��
��    �������� type 1  67��
�� �� ֵ��0�ɹ�   1ʧ��
��    ע��ͬ��ͷ:9ms�͵�ƽ+4.5ms�ߵ�ƽ
		  �߼�1��560us��+1680us��
		  �߼�0��560us��+560us��
*****************************************************/
u8 Infrared_AnalysisNECCode(u8 type)
{
	switch (type)
	{
	case 1:




		break;
	}
	return 0;
}








