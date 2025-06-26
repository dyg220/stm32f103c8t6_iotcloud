#include "main.h"

int main()
{
	LED_Init();
	Key_Init();
	Relay_Init();
	USART1_Init();

	while (1)
	{
		/*u8 key_num = Key_Scan();
		if (key_num == key1_click)
		{
			RELAY_ON;

		}
		else if (key_num == key2_click)
		{
			RELAY_OFF;
		}*/

		USART1_SendByte('A');
		delay_ms(1000);

	}
	return 0;
}

