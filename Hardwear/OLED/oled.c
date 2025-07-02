#include "oled.h"
#include "spi2.h"

void OLED_Pin_Init(void)
{
	// 开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	// 配置GPIO
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	SPI2_Config();

	OLED_CS_H;
	OLED_RES_H;
	OLED_DC_H;

	delay_us(500);
}
/****************************************************
函数功能：OLED写命令
参    数：None
返 回 值：None
备    注：
*****************************************************/
void OLED_WriteCommand(u8 Command)
{
	OLED_DC_L;              // 拉低DC,表示写命令
	OLED_CS_L;              // 拉低CS,表示通信开始
	SPI2_Transmit(Command); // 写入指定命令
	OLED_CS_H;              // 拉高CS,表示通信结束
}

/****************************************************
函数功能：OLED写数据
参    数：None
返 回 值：None
备    注：
*****************************************************/
void OLED_WriteData(u8 Data)
{
	OLED_DC_H;           // 拉低DC,表示写数据
	OLED_CS_L;           // 拉低DC,表示通信开始
	SPI2_Transmit(Data); // 写入指定命令
	OLED_CS_H;           // 拉高DC,表示通信结束
}

/****************************************************
函数功能：OLED初始化
参    数：None
返 回 值：None
备    注：OLED驱动序列---厂家提供
*****************************************************/
void OLED_Init(void)
{
	OLED_Pin_Init(); // 引脚初始化
	OLED_RES_H;      // 复位
	delay_ms(100);
	OLED_RES_L; // 复位
	delay_ms(100);
	OLED_RES_H;

	/*写入一系列的命令，对OLED进行初始化配置*/
	OLED_WriteCommand(0xAE); // 设置显示开启/关闭，0xAE关闭，0xAF开启

	OLED_WriteCommand(0xD5); // 设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80); // 0x00~0xFF

	OLED_WriteCommand(0xA8); // 设置多路复用率
	OLED_WriteCommand(0x3F); // 0x0E~0x3F

	OLED_WriteCommand(0xD3); // 设置显示偏移
	OLED_WriteCommand(0x00); // 0x00~0x7F

	OLED_WriteCommand(0x40); // 设置显示开始行，0x40~0x7F

	OLED_WriteCommand(0xA1); // 设置左右方向，0xA1正常，0xA0左右反置

	OLED_WriteCommand(0xC8); // 设置上下方向，0xC8正常，0xC0上下反置

	OLED_WriteCommand(0xDA); // 设置COM引脚硬件配置
	OLED_WriteCommand(0x12);

	OLED_WriteCommand(0x81); // 设置对比度
	OLED_WriteCommand(0xCF); // 0x00~0xFF

	OLED_WriteCommand(0xD9); // 设置预充电周期
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB); // 设置VCOMH取消选择级别
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4); // 设置整个显示打开/关闭

	OLED_WriteCommand(0xA6); // 设置正常/反色显示，0xA6正常，0xA7反色

	OLED_WriteCommand(0x8D); // 设置充电泵
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF); // 开启显示

	OLED_Clear(); // 清屏
}

// 设置光标(起始位置)
void OLED_setCursor(u8 X, u8 Page)
{
	if (Page > 7 || X > 127) return;           // 范围
	OLED_WriteCommand(0xB0 | Page);            // 发送页地址
	OLED_WriteCommand(0x00 | (X & 0x0F));      // 发送x列的低四位
	OLED_WriteCommand(0x10 | (X & 0xF0) >> 4); // 发送x列的高四位
}

void OLED_Clear(void)
{
	u8 i = 0, j = 0;
	for (j = 0; j < 8; j++)
	{
		OLED_setCursor(0, j);
		for (i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}
