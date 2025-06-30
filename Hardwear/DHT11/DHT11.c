#include "dht11.h"

/****************************************************
�������ܣ�DHT11��ʼ������
��    ����None
�� �� ֵ��None
��    ע��PB3---DHT11_DATA
*****************************************************/
void DHT11_Init(void)
{
	//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//���JTAG

	//����GPIO
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	delay_ms(1000);//Խ�����ȶ�״̬
	DHT11_DATA_H;//����״̬
	delay_us(5);

}

/****************************************************
�������ܣ���ʼ�ź�
��    ����None
�� �� ֵ��None
��    ע��������������20ms
*****************************************************/
void DHT11_Start(void)
{
	DHT11_DATA_L;
	delay_ms(20);
	DHT11_DATA_H;
}

/****************************************************
�������ܣ���Ӧ�ź�
��    ����None
�� �� ֵ����Ӧ�ɹ� ����0
��    ע��DHT11����83us+����87us
*****************************************************/
u8 DHT11_Respond(void)
{
	u8 timeout = 0;
	while (DHT11_DATA_R == 1)//�ȴ��ӻ���������
	{
		timeout++;
		delay_us(1);
		if (timeout > 50) return 1;
	}
	timeout = 0;
	while (DHT11_DATA_R == 0)//�ȴ��ӻ���Ӧ�ź�83us��ȥ
	{
		timeout++;
		delay_us(1);
		if (timeout > 100) return 2;
	}
	timeout = 0;
	while (DHT11_DATA_R == 1)//�ȴ��ӻ�֪ͨ�ź�87us��ȥ
	{
		timeout++;
		delay_us(1);
		if (timeout > 120) return 3;
	}
	return 0;
}

/****************************************************
�������ܣ���ȡ40λ����
��    ����None
�� �� ֵ���ɹ�0
��    ע��
*****************************************************/
u8 DHT11_ReadData(u8* Data)
{
	u8 i = 0, j = 0;
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 8; j++)
		{
			while (DHT11_DATA_R == 0);//����54us�ĵ͵�ƽ
			delay_us(50);//��ʱ50us
			Data[i] <<= 1;//�ճ������λ����λ�ȷ�
			if (DHT11_DATA_R == 1)//�����Ǹߵ�ƽ
			{
				Data[i] |= 0x01;
				while (DHT11_DATA_R == 1);//����ʣ��ߵ�ƽ
			}
		}
	}
	return 0;
}


/****************************************************
�������ܣ�������ȡ��ʪ��
��    �����¶�: float temp    ʪ��:u8 humi
�� �� ֵ���ɹ�0
��    ע��
*****************************************************/
u8 DHT11_GetTempHumi(float* temp, u8* humi)
{
	u8 ret = 0;
	u8 Data[5] = { 0 };
	DHT11_Start();//��ʼ�ź�
	ret = DHT11_Respond();//��Ӧ�ź�
	if (ret) return 1;
	ret = DHT11_ReadData(Data);//���ݽ���
	if (ret) return 2;
	while (DHT11_DATA_R == 0);//����54us�ĵ͵�ƽ(�����źţ�

	if (((Data[0] + Data[1] + Data[2] + Data[3]) & 0xff) != Data[4])
		return 3;
	*humi = Data[0];
	if (Data[3] & 0x80)//���
	{
		*temp = -1 * (Data[2] + (Data[3] & 0x7f) / 10.0f);//
	}
	else
	{
		*temp = Data[2] + Data[3] / 10.0f;
	}
	return 0;
}
