#include "main.h"

float Temp = 0;
u8 Humi = 0;

int main()
{
	// u8 key_num = 0;
	u8 ret = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); // 关系到配置抢占和响应数字的范围
	LED_Init();
	Key_Init();
	Relay_Init();
	USART1_Config(115200);
	// dcMotor_Config();
	//StepMotor_Init();
	// USART1_SendStr((u8*)"Hello, USART1!\r\n");
	RGB_Init();
	DHT11_Init();
	printf("Reset!!!\r\n");

	// RGB_Control(0, 0, 0xff);
	//StepMotor_Control(100, 1);
	while (1)
	{
		ret = DHT11_GetTempHumi(&Temp, &Humi);
		if (ret)
		{
			printf("ret = %d\r\n", ret);
		}
		else
		{
			printf("Temp：%.1f℃   Humi：%d%%\r\n", Temp, Humi);
		}
		delay_ms(2000);

		// key_num = Key_Scan();
		// if (key_num == key1_click)
		//{
		//	//DCMotor_SetSpeed(100);
		// }
		// else if (key_num == key2_click)
		//{
		//	//DCMotor_SetSpeed(-100);
		// }
		// else if (key_num == key3_click)
		//{
		//	//DCMotor_SetSpeed(0);
		// }
		RECV_USART1(); // 接收USART1数据
	}
	return 0;
}
