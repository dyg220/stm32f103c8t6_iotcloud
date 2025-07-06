#include "main.h"

float Temp = 0;
u8 Humi = 0;

int main()
{
	//u8 key_num = 0;
	//u8 ret = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); // 关系到配置抢占和响应数字的范围
	LED_Init();
	Key_Init();
	Relay_Init();
	USART1_Config(115200);
	USART2_Config(115200);
	// dcMotor_Config();
	// StepMotor_Init();
	// USART1_SendStr((u8*)"Hello, USART1!\r\n");
	RGB_Init();
	DHT11_Init();
	Infrared_Init();
	OLED_Init();
	//OLED_ShowNum(0, 0, 1234567890, 10, OLED_8X16);
	//OLED_ShowImage(0, 16, 16, 16, Diode);
	OLED_ShowString(0, 0, "1234567890123456", OLED_8X16);
	OLED_ShowString(0, 16, "1234567890123456", OLED_8X16);
	OLED_ShowString(0, 32, "你好", OLED_8X16);
	OLED_Update();
	printf("Reset!!!\r\n");
	// RGB_Control(0, 0, 0xff);
	while (1)
	{
		// StepMotor_Control(100, 1);
	/*	ret = DHT11_GetTempHumi(&Temp, &Humi);
		if (ret)
		{
			printf("ret = %d\r\n", ret);
		}
		else
		{
			printf("Temp：%.1f℃   Humi：%d%%\r\n", Temp, Humi);
		}
		delay_ms(2000);*/

		if (Infrared_info_t.CaptureOver == 1) // 采集结束
		{
			Infrared_info_t.CaptureOver = 0;
			printf("捕获的边沿数：%d\r\n", Infrared_info_t.EdgeCnt);
			for (u8 i = 0; i < Infrared_info_t.EdgeCnt; i++)
			{
				printf("%d ", Infrared_info_t.TimeCnt[i]);
			}
			printf("\r\n");
			Infrared_AnalysisNECBit(1);
		}

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
