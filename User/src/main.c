#include "main.h"

int main()
{
	//u8 key_num = 0;
	uint8_t temp, humi;              // 定义变量用于存储温度和湿度值
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); // 关系到配置抢占和响应数字的范围
	LED_Init();
	Key_Init();
	Relay_Init();
	USART1_Config(115200);
	//dcMotor_Config();
	//USART1_SendStr((u8*)"Hello, USART1!\r\n");
	printf("Reset!!!\r\n");
	RGB_Init();
	//RGB_Control(0, 0, 0xff);

	 // 等待DHT11温湿度稳定
	delay_ms(1000);                      // 延时1秒，确保DHT11传感器稳定并准备好接收指令

	// 发送启动信号
	DHT11_Start();                       // 向DHT11发送启动信号，唤醒传感器

	// 检测DHT11的响应
	if (!DHT11_CheckResponse())          // 检查DHT11是否正确响应
	{
		printf("DHT11 Error \r\n");		// 如果未响应，通过串口打印错误信息
		delay_ms(1000);                  // 延时1秒
		return 1;                        // 返回错误代码1，程序退出
	}
	printf("DHT11 Start \r\n");			// 如果响应成功，通过串口打印启动信息

	while (1)
	{
		// 读取温湿度数据
		DHT11_ReadData(&temp, &humi);    // 调用DHT11驱动函数，读取温度和湿度值

		// 打印温湿度
		printf("temp %d ,humi %d\r\n", temp, humi); // 通过串口打印温度和湿度值

		// 延时1秒
		delay_ms(1000);                  // 每次读取后延时1秒，避免频繁读取
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
		RECV_USART1(); // 接收USART1数据
	}
	return 0;
}

