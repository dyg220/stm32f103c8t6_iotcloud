#include "DHT11.h"


void DHT11_Start(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //禁用JTAG，启用SWD
	GPIO_InitTypeDef GPIO_InitStructure;
	// 配置为推挽输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// 拉低至少18ms
	GPIO_ResetBits(GPIOB, GPIO_Pin_3);
	delay_ms(18); // 延时18ms

	// 拉高20~40us
	GPIO_SetBits(GPIOB, GPIO_Pin_3);
	delay_us(30); // 延时30us
}

uint8_t DHT11_CheckResponse(void)
{
	uint8_t response = 0;
	// 切换为输入模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//等待DHT11拉低信号
	while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)); //等待信号线由高变低

	//等待DHT11拉高信号
	while (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)); //等待信号线由低变高

	//检测拉高时间是否为80us
	delay_us(80);		//延时80us
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)) {
		response = 1; //响应成功
	}

	return response;
}
//读取一位数据位
uint8_t DHT11_ReadBit(void)
{
	uint8_t bit = 0;
	while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)); // 等待信号线由高变低
	delay_us(40); // 延时40us，确保信号稳定，数据“0”时隙并不是准确 26~28us，可能比这短，也可能比这长

	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)) {
		bit = 1; // 如果信号线拉高超过40us，则为1
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)); // 等待信号线由高变低
	}
	return bit;
}

//读取一个字节数据
uint8_t DHT11_ReadByte(void)
{
	uint8_t byte = 0;
	for (uint8_t i = 0; i < 8; i++) {
		byte <<= 1; // 左移一位
		byte |= DHT11_ReadBit(); // 读取一位数据
	}
	return byte;
}


//获取外界温湿度
uint8_t DHT11_ReadData(uint8_t* humidity, uint8_t* temperature)
{
	uint8_t data[5]; // 40位数据，5个字节
	uint8_t checksum;

	// 读取40位数据
	for (uint8_t i = 0; i < 5; i++)
	{
		data[i] = DHT11_ReadByte();
	}

	// 校验数据
	checksum = data[0] + data[1] + data[2] + data[3];
	if (checksum != data[4])
	{
		return 1; // 校验失败
	}

	// 提取温湿度数据
	*humidity = data[0]; // 湿度整数
	*temperature = data[2]; // 温度整数
	return 0; // 成功
}

