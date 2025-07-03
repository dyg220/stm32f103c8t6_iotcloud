#include "spi2.h"	

void SPI2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 // Enable GPIOA clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15; // SCK, MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			// Alternate function push-pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// Set speed
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	SPI_InitTypeDef SPI_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);				//Enable SPI1 clock
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // Full duplex
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		 // Set as master
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // 8-bit data size
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//时钟极性
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//时钟相位
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		// Software NSS management
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;	// 2分频 最高36Mhz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; // MSB first
	SPI_Init(SPI2, &SPI_InitStructure);

	SPI_Cmd(SPI2, ENABLE);

}


u8 SPI2_Transmit(u8 Byte)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);  //发送区为空
	SPI_I2S_SendData(SPI2, Byte);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); //等待接收完成
	return SPI_I2S_ReceiveData(SPI2);			//返回接收到的数据
}