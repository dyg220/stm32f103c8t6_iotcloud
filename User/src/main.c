#include "main.h"

int main()
{
	//u8 key_num = 0;
	LED_Init();
	Key_Init();
	Relay_Init();
	USART1_Config(115200);
	//USART1_SendStr((u8*)"Hello, USART1!\r\n");
	printf("hello");
	while (1)
	{
		/* key_num = Key_Scan();
		if (key_num == key1_click)
		{
			RELAY_ON;

		}
		else if (key_num == key2_click)
		{
			RELAY_OFF;
		}*/

		//USART1_SendByte('A');


	}
	return 0;
}

