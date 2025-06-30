#include "DHT11.h"


void DHT11_Start(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //����JTAG������SWD
	GPIO_InitTypeDef GPIO_InitStructure;
	// ����Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// ��������18ms
	GPIO_ResetBits(GPIOB, GPIO_Pin_3);
	delay_ms(18); // ��ʱ18ms

	// ����20~40us
	GPIO_SetBits(GPIOB, GPIO_Pin_3);
	delay_us(30); // ��ʱ30us
}

uint8_t DHT11_CheckResponse(void)
{
	uint8_t response = 0;
	// �л�Ϊ����ģʽ
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//�ȴ�DHT11�����ź�
	while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)); //�ȴ��ź����ɸ߱��

	//�ȴ�DHT11�����ź�
	while (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)); //�ȴ��ź����ɵͱ��

	//�������ʱ���Ƿ�Ϊ80us
	delay_us(80);		//��ʱ80us
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)) {
		response = 1; //��Ӧ�ɹ�
	}

	return response;
}
//��ȡһλ����λ
uint8_t DHT11_ReadBit(void)
{
	uint8_t bit = 0;
	while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)); // �ȴ��ź����ɸ߱��
	delay_us(40); // ��ʱ40us��ȷ���ź��ȶ������ݡ�0��ʱ϶������׼ȷ 26~28us�����ܱ���̣�Ҳ���ܱ��ⳤ

	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)) {
		bit = 1; // ����ź������߳���40us����Ϊ1
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)); // �ȴ��ź����ɸ߱��
	}
	return bit;
}

//��ȡһ���ֽ�����
uint8_t DHT11_ReadByte(void)
{
	uint8_t byte = 0;
	for (uint8_t i = 0; i < 8; i++) {
		byte <<= 1; // ����һλ
		byte |= DHT11_ReadBit(); // ��ȡһλ����
	}
	return byte;
}


//��ȡ�����ʪ��
uint8_t DHT11_ReadData(uint8_t* humidity, uint8_t* temperature)
{
	uint8_t data[5]; // 40λ���ݣ�5���ֽ�
	uint8_t checksum;

	// ��ȡ40λ����
	for (uint8_t i = 0; i < 5; i++)
	{
		data[i] = DHT11_ReadByte();
	}

	// У������
	checksum = data[0] + data[1] + data[2] + data[3];
	if (checksum != data[4])
	{
		return 1; // У��ʧ��
	}

	// ��ȡ��ʪ������
	*humidity = data[0]; // ʪ������
	*temperature = data[2]; // �¶�����
	return 0; // �ɹ�
}

