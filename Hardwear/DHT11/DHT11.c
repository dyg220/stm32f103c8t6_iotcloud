#include "dht11.h"

/****************************************************
函数功能：DHT11初始化函数
参    数：None
返 回 值：None
备    注：PB3---DHT11_DATA
*****************************************************/
void DHT11_Init(void)
{
	// 开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); // 解除JTAG

	// 配置GPIO
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	delay_ms(1000); // 越过不稳定状态
	DHT11_DATA_H;   // 空闲状态
	delay_us(5);
}

/****************************************************
函数功能：起始信号
参    数：None
返 回 值：None
备    注：主机拉低总线20ms
*****************************************************/
void DHT11_Start(void)
{
	DHT11_DATA_L;
	delay_ms(20);
	DHT11_DATA_H;
}

/****************************************************
函数功能：响应信号
参    数：None
返 回 值：响应成功 返回0
备    注：DHT11拉低83us+拉高87us
*****************************************************/
u8 DHT11_Respond(void)
{
	u8 timeout = 0;
	while (DHT11_DATA_R == 1) // 等待从机拉低总线
	{
		timeout++;
		delay_us(1);
		if (timeout > 50)
			return 1;
	}
	timeout = 0;
	while (DHT11_DATA_R == 0) // 等待从机响应信号83us过去
	{
		timeout++;
		delay_us(1);
		if (timeout > 100)
			return 2;
	}
	timeout = 0;
	while (DHT11_DATA_R == 1) // 等待从机通知信号87us过去
	{
		timeout++;
		delay_us(1);
		if (timeout > 120)
			return 3;
	}
	return 0;
}

/****************************************************
函数功能：获取40位数据
参    数：None
返 回 值：成功0
备    注：
*****************************************************/
u8 DHT11_ReadData(u8* Data)
{
	u8 i = 0, j = 0;
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 8; j++)
		{
			while (DHT11_DATA_R == 0)
				;                  // 过滤54us的低电平
			delay_us(50);          // 延时50us
			Data[i] <<= 1;         // 空出来最低位，高位先发
			if (DHT11_DATA_R == 1) // 若仍是高电平
			{
				Data[i] |= 0x01;
				while (DHT11_DATA_R == 1)
					; // 过滤剩余高电平
			}
		}
	}
	return 0;
}

/****************************************************
函数功能：主机获取温湿度
参    数：温度: float temp    湿度:u8 humi
返 回 值：成功0
备    注：
*****************************************************/
u8 DHT11_GetTempHumi(float* temp, u8* humi)
{
	u8 ret = 0;
	u8 Data[5] = { 0 };
	DHT11_Start();         // 起始信号
	ret = DHT11_Respond(); // 响应信号
	if (ret)
		return 1;
	ret = DHT11_ReadData(Data); // 数据接收
	if (ret)
		return 2;
	while (DHT11_DATA_R == 0)
		; // 过滤54us的低电平(结束信号）

	if (((Data[0] + Data[1] + Data[2] + Data[3]) & 0xff) != Data[4])
		return 3;
	*humi = Data[0];
	if (Data[3] & 0x80) // 如果
	{
		*temp = -1 * (Data[2] + (Data[3] & 0x7f) / 10.0f); //
	}
	else
	{
		*temp = Data[2] + Data[3] / 10.0f;
	}
	return 0;
}
