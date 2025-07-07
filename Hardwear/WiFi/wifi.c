#include "wifi.h"

volatile u8 WiFi_Flag = 0;

extern  WiFi_INFO WiFi_Rev;
/****************************************************
 * 函数名：WiFi_Init
 * 函数功能：WiFi初始化
 * 参    数：None
 * 返 回 值：None
 ****************************************************/
void WiFi_Init(void)
{
	u8 ret = 0;
	char WiFi_Name[32] = "tatt8990";
	char WiFi_Pwd[32] = "22336699";
	char CliendID[64] = "123|securemode=3\\,signmethod=hmacsha1|";
	char Username[64] = "SamarHomeV1.0&a1sQGf30de7";
	char Password[64] = "EC8EFA2B87BE190DA84F0895A2D2ECDD51E0CFB4";
	char ServerIP[32] = "203.107.45.14";
	char Sub[64] = "/sys/a1sQGf30de7/SamarHomeV1.0/thing/service/property/set";

	WiFi_SendAT("AT+RST\r\n", "ready", 2000);
	WiFi_SendAT("AT+CWAUTOCONN=0\r\n", "OK", 2000);//取消WiFi自动连接
	WiFi_SendAT("AT+MQTTCLEAN=0\r\n", "OK", 1000);//取消MQTT连接
	WiFi_NOSubscription(Sub);//取消订阅
	WiFi_SendAT("AT+CWMODE=1\r\n", "OK", 2000);//设置WiFi模式

	ret = WiFi_ConnectAP(WiFi_Name, WiFi_Pwd);//连接WiFi
	if (ret) printf("----------WiFi连接失败-----------\r\n");
	delay_ms(500);

	ret = WiFi_SetMQTT(CliendID, Username, Password);//设置MQTT
	if (ret) printf("----------设置MQTT失败-----------\r\n");
	delay_ms(500);

	do
	{
		ret = WiFi_SendDomain(ServerIP, 1883);//发送域名
		if (ret) printf("----------发送域名失败-----------\r\n");
	} while (ret);

	ret = WiFi_Subscription(Sub);//订阅
	if (ret)printf("-----订阅错误-----\r\n");
	delay_ms(500);

	//	do
	//	{
	//		ret = WiFi_SetTime("ntp1.aliyun.com");//设置时域
	//		if(ret) printf("----------设置时域失败-----------\r\n");
	//	}while(ret);

	WiFi_Flag = 1;
}




/**********************************************************
*函数功能：发送AT指令
*参    数：待发送的指令   期望的AT响应
*返 回 值：0成功    1失败
*备    注：
***********************************************************/
u8 WiFi_SendAT(char* AT, char* respond, u16 TimeOut)
{
	u16 TimeCnt = 0;
	memset(WiFi_Rev.r_buf, 0, sizeof(WiFi_Rev.r_buf));//清空数组
	WiFi_Rev.r_len = 0;
	USART2_SendStr(AT);
	while (1)
	{
		delay_ms(1);
		TimeCnt++;
		if (TimeCnt > TimeOut) return 1;
		if (WiFi_Rev.r_Flag == 1)
		{
			WiFi_Rev.r_Flag = 0;
			if (strstr(WiFi_Rev.r_buf, respond) != NULL)//如果收到想要的回复
			{
				WiFi_Rev.r_buf[WiFi_Rev.r_len] = '\0';
				printf("%s\r\n", WiFi_Rev.r_buf);
				WiFi_Rev.r_len = 0;
				memset(WiFi_Rev.r_buf, 0, sizeof(WiFi_Rev.r_buf));//清空数组
				return 0;
			}
		}
	}
}

/**********************************************************
*函数功能：模块连接热点
*参    数：热点名称ssid   热点密码pwd
*返 回 值：0 成功 其他失败
*备    注：
***********************************************************/
u8 WiFi_ConnectAP(char* ssid, char* pwd)
{
	u8 ret = 0;
	char buf[128];
	sprintf(buf, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, pwd);
	ret = WiFi_SendAT(buf, "OK", 5000);
	if (ret) return 1;
	return 0;
}
/**********************************************************
*函数功能：设置时间域
*参    数：服务器
*返 回 值：0 成功 其他失败
*备    注：ClientID 中的逗号前面加一个\
***********************************************************/
u8 WiFi_SetTime(char* Server)
{
	u8 ret = 0;
	char buf[128];
	sprintf(buf, "AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"");
	ret = WiFi_SendAT(buf, "OK", 2000);
	if (ret) return 1;
	return 0;
}


/**********************************************************
*函数功能：设置MQTT属性
*参    数：ClientID    Username   Password
*返 回 值：0 成功 其他失败
*备    注：ClientID 中的逗号前面加一个\
***********************************************************/
u8 WiFi_SetMQTT(char* ClientID, char* Username, char* Password)
{
	u8 ret;
	char buf[256];
	sprintf(buf, "AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n", ClientID, Username, Password);
	ret = WiFi_SendAT(buf, "OK", 10000);
	if (ret)
		return 1;
	return 0;
}

/**********************************************************
*函数功能：发送域名
*参    数：None
*返 回 值：0 成功 其他失败
*备    注：
***********************************************************/
u8 WiFi_SendDomain(char* IP, u16 port)
{
	u8 ret;
	char buf[128];
	sprintf(buf, "AT+MQTTCONN=0,\"%s\",%d,1\r\n", IP, port);
	ret = WiFi_SendAT(buf, "OK", 8000);
	if (ret)
		return 1;
	return 0;
}

/**********************************************************
*函数功能：订阅
*参    数：None
*返 回 值：None
*备    注：
***********************************************************/
u8 WiFi_Subscription(char* SUB)
{
	u8 ret;
	char buf[128];
	sprintf(buf, "AT+MQTTSUB=0,\"%s\",1\r\n", SUB);
	ret = WiFi_SendAT(buf, "OK", 8000);
	if (ret)
		return 1;
	return 0;
}

/**********************************************************
*函数功能：取消订阅
*参    数：None
*返 回 值：None
*备    注：
***********************************************************/
u8 WiFi_NOSubscription(char* SUB)
{
	u8 ret;
	char buf[128];
	sprintf(buf, "AT+MQTTUNSUB=0,\"%s\"\r\n", SUB);
	ret = WiFi_SendAT(buf, "OK", 8000);
	if (ret)
		return 1;
	return 0;
}

/**********************************************************
*函数功能：发布消息
*参    数：None
*返 回 值：None
*备    注：
***********************************************************/
void WiFi_Issue(void)
{

}



/**********************************************************
*函数功能：数据包解析
*参    数：None
*返 回 值：None
*备    注：
***********************************************************/
void Package_Analysis(void)
{


}









