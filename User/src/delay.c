#include "delay.h"

/*for (uint32_t i = 0; i < 72000 ; i++)
{
	for (uint32_t j = 0; j < 500; j++)
	{
		__asm("nop");
	}
}*/

void delay_us(uint32_t xus)
{
	while (xus--)
	{
		__ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); \
			__ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); \
			__ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); \
			__ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); \
			__ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); \
			__ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); \
			__ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); __ASM("nop"); \
			__ASM("nop"); __ASM("nop");
	}
}

void delay_ms(uint32_t xms)
{
	while (xms--)
	{
		delay_us(1000);
	}
}
