#include "main.h"

int main()
{
	//u8 key_num = 0;
	uint8_t temp, humi;              // ����������ڴ洢�¶Ⱥ�ʪ��ֵ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); // ��ϵ��������ռ����Ӧ���ֵķ�Χ
	LED_Init();
	Key_Init();
	Relay_Init();
	USART1_Config(115200);
	//dcMotor_Config();
	//USART1_SendStr((u8*)"Hello, USART1!\r\n");
	printf("Reset!!!\r\n");
	RGB_Init();
	//RGB_Control(0, 0, 0xff);

	 // �ȴ�DHT11��ʪ���ȶ�
	delay_ms(1000);                      // ��ʱ1�룬ȷ��DHT11�������ȶ���׼���ý���ָ��

	// ���������ź�
	DHT11_Start();                       // ��DHT11���������źţ����Ѵ�����

	// ���DHT11����Ӧ
	if (!DHT11_CheckResponse())          // ���DHT11�Ƿ���ȷ��Ӧ
	{
		printf("DHT11 Error \r\n");		// ���δ��Ӧ��ͨ�����ڴ�ӡ������Ϣ
		delay_ms(1000);                  // ��ʱ1��
		return 1;                        // ���ش������1�������˳�
	}
	printf("DHT11 Start \r\n");			// �����Ӧ�ɹ���ͨ�����ڴ�ӡ������Ϣ

	while (1)
	{
		// ��ȡ��ʪ������
		DHT11_ReadData(&temp, &humi);    // ����DHT11������������ȡ�¶Ⱥ�ʪ��ֵ

		// ��ӡ��ʪ��
		printf("temp %d ,humi %d\r\n", temp, humi); // ͨ�����ڴ�ӡ�¶Ⱥ�ʪ��ֵ

		// ��ʱ1��
		delay_ms(1000);                  // ÿ�ζ�ȡ����ʱ1�룬����Ƶ����ȡ
		//key_num = Key_Scan();
		//if (key_num == key1_click)
		//{
		//	//DCMotor_SetSpeed(100);
		//}
		//else if (key_num == key2_click)
		//{
		//	//DCMotor_SetSpeed(-100);
		//}
		//else if (key_num == key3_click)
		//{
		//	//DCMotor_SetSpeed(0);
		//}
		RECV_USART1(); // ����USART1����
	}
	return 0;
}

