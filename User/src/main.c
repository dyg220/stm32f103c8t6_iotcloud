#include "main.h"

int main()
{
	LED_Init();
	Key_Init();
	Relay_Init();

	while (1)
	{
		u8 key_num = Key_Scan();
		if (key_num == key1_click)
		{
			RELAY_ON;
		}
		else if (key_num == key1_long)
		{
			RELAY_OFF;
		}
	}
	return 0;
}
