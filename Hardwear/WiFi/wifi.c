#include "wifi.h"
#include "usart2.h"

/****************************************************
 * 函数名：WiFi_Init
 * 函数功能：WiFi初始化
 * 参    数：None
 * 返 回 值：None
 ****************************************************/
void WiFi_Init(void)
{
	// 初始化WiFi模块
	// 例如，设置波特率、配置GPIO等
	USART2_Config(115200); // 设置USART2波特率为115200
	// 其他初始化代码...

	// 发送AT指令测试WiFi模块是否正常工作
	//WiFi_SendAT("AT\r\n", "OK", 1000);
	WiFi_SendAT("AT+CWMODE=1\r\n", "OK", 1000);
	WiFi_SendAT("AT+CWJAP=\"tatt8990\",\"22336699\"\r\n", "OK", 2000);
	WiFi_SendAT("AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"\r\n\"", "OK", 5000);
	WiFi_SendAT("AT+MQTTUSERCFG=0,1,\"123|securemode=3\\,signmethod =hmacsha1|\",\"SamarHomeV1.0&a1sQGf30de7\",\"EC8EFA2B87BE190DA84F0895A2D2ECDD51E0CFB4\",0,0,\"\"\r\n", "OK", 10000);
	WiFi_SendAT("AT+MQTTCONN=0,\"a1sQGf30de7.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883,1\r\n", "OK", 2000);



}

/****************************************************
 * 函 数 名：WiFi_SendAT
 * 函数功能：WiFi发送AT指令
 * 参    数：ATCmd：AT指令 respond：期望的响应  timeout：超时时间
 * 返 回 值：None
 ****************************************************/
void WiFi_SendAT(char* AT, char* respond, u32 TimeOut)
{
	USART2_SendStr(AT); // 发送AT指令
	delay_ms(100);      // 等待模块响应
	// 等待响应
	u32 TimeCnt = 0;
	while (TimeCnt < TimeOut)
	{
		if (WiFi_Rev.r_Flag) // 如果接收到数据
		{
			if (strstr((char*)WiFi_Rev.r_buf, respond) != NULL) // 检查是否包含期望的响应
			{
				printf("Response received: %s\n", WiFi_Rev.r_buf);
				WiFi_Rev.r_Flag = 0; // 重置标志
				return;               // 成功接收响应
			}
			WiFi_Rev.r_Flag = 0; // 重置标志
		}
		TimeCnt++;   // 增加已用时间
		delay_ms(2000); // 等待一段时间再检查
	}
	printf("No response or timeout.\n"); // 超时或未收到响应
}
