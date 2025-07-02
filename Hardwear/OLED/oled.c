#include "oled.h"	

void OLED_Init(void)
{
	// Initialize GPIO pins for SPI communication	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // Enable GPIOA clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15 | GPIO_Pin_7; // SCK, MOSI, CS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // Alternate function push-pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // Set speed
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// Initialize SPI	
	SPI_InitTypeDef SPI_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); // Enable SPI1 clock
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // Full duplex
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; // Set as master
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // 8-bit data size
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // Clock polarity low
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; // Clock phase on first edge
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard; // Software NSS management
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; // Prescaler
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; // MSB first
	SPI_InitStructure.SPI_CRCPolynomial = 7; // CRC polynomial

	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE); // Enable SPI
	// Initialize OLED display
	OLED_WriteCommand(0xAE); // Display off
	OLED_WriteCommand(0xD5); // Set display clock divide ratio/oscillator frequency
	OLED_WriteCommand(0x80); // Set divide ratio
	OLED_WriteCommand(0xA8); // Set multiplex ratio
	OLED_WriteCommand(0x3F); // 1/64 duty
	OLED_WriteCommand(0xD3); // Set display offset
	OLED_WriteCommand(0x00); // No offset
	OLED_WriteCommand(0x40); // Set display start line to 0
	OLED_WriteCommand(0x8D); // Charge pump setting
	OLED_WriteCommand(0x14); // Enable charge pump
	OLED_WriteCommand(0x20); // Set memory addressing mode
	OLED_WriteCommand(0x00); // Horizontal addressing mode
	OLED_WriteCommand(0xA1); // Set segment re-map
	OLED_WriteCommand(0xC8); // Set COM output scan direction
	OLED_WriteCommand(0xDA); // Set COM pins hardware configuration
	OLED_WriteCommand(0x12); // Alternative COM pin configuration
	OLED_WriteCommand(0x81); // Set contrast control
	OLED_WriteCommand(0xCF); // Set contrast to maximum
	OLED_WriteCommand(0xD9); // Set pre-charge period
	OLED_WriteCommand(0xF1); // Set pre-charge to 15 DCLKs
	OLED_WriteCommand(0xDB); // Set VCOMH deselect level
	OLED_WriteCommand(0x40); // Set VCOMH to 0.77 * VCC
	OLED_WriteCommand(0xA4); // Disable entire display on
	OLED_WriteCommand(0xA6); // Set normal display
	OLED_WriteCommand(0xAF); // Display on
}

void OLED_WriteCommand(uint8_t cmd) {
	GPIO_ResetBits(GPIOB, GPIO_Pin_7); // Set CS low to select OLED
	GPIO_ResetBits(GPIOB, GPIO_Pin_13); // Set DC low for command mode
	SPI_I2S_SendData(SPI2, cmd); // Send command
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY)); // Wait until transmission is complete
	GPIO_SetBits(GPIOB, GPIO_Pin_7); // Set CS high to deselect OLED
}