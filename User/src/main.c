#include "main.h"

int main()
{
	u8 key_num = 0;
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
	while (1)
	{
		key_num = Key_Scan();
		if (key_num == key1_click)
		{
			//DCMotor_SetSpeed(100);
		}
		else if (key_num == key2_click)
		{
			//DCMotor_SetSpeed(-100);
		}
		else if (key_num == key3_click)
		{
			//DCMotor_SetSpeed(0);
		}
		RECV_USART1(); // 接收USART1数据
	}
	return 0;
}


