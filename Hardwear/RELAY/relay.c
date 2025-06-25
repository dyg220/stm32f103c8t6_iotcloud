#include "relay.h"

// PB1: 高电平导通继电器
void Relay_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	// Configure GPIOB Pin 11 as output push-pull
	GPIO_InitTypeDef GPIO_InitStructure = {
		.GPIO_Pin = GPIO_Pin_1,        // Pin 1
		.GPIO_Mode = GPIO_Mode_Out_PP, // Output mode
		.GPIO_Speed = GPIO_Speed_2MHz  // Speed 50MHz
	};

	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
