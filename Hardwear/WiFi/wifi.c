#include "wifi.h"

volatile u8 WiFi_Flag = 0;

extern  WiFi_INFO WiFi_Rev;
/****************************************************
 * ��������WiFi_Init
 * �������ܣ�WiFi��ʼ��
 * ��    ����None
 * �� �� ֵ��None
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
	WiFi_SendAT("AT+CWAUTOCONN=0\r\n", "OK", 2000);//ȡ��WiFi�Զ�����
	WiFi_SendAT("AT+MQTTCLEAN=0\r\n", "OK", 1000);//ȡ��MQTT����
	WiFi_NOSubscription(Sub);//ȡ������
	WiFi_SendAT("AT+CWMODE=1\r\n", "OK", 2000);//����WiFiģʽ

	ret = WiFi_ConnectAP(WiFi_Name, WiFi_Pwd);//����WiFi
	if (ret) printf("----------WiFi����ʧ��-----------\r\n");
	delay_ms(500);

	ret = WiFi_SetMQTT(CliendID, Username, Password);//����MQTT
	if (ret) printf("----------����MQTTʧ��-----------\r\n");
	delay_ms(500);

	do
	{
		ret = WiFi_SendDomain(ServerIP, 1883);//��������
		if (ret) printf("----------��������ʧ��-----------\r\n");
	} while (ret);

	ret = WiFi_Subscription(Sub);//����
	if (ret)printf("-----���Ĵ���-----\r\n");
	delay_ms(500);

	//	do
	//	{
	//		ret = WiFi_SetTime("ntp1.aliyun.com");//����ʱ��
	//		if(ret) printf("----------����ʱ��ʧ��-----------\r\n");
	//	}while(ret);

	WiFi_Flag = 1;
}




/**********************************************************
*�������ܣ�����ATָ��
*��    ���������͵�ָ��   ������AT��Ӧ
*�� �� ֵ��0�ɹ�    1ʧ��
*��    ע��
***********************************************************/
u8 WiFi_SendAT(char* AT, char* respond, u16 TimeOut)
{
	u16 TimeCnt = 0;
	memset(WiFi_Rev.r_buf, 0, sizeof(WiFi_Rev.r_buf));//�������
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
			if (strstr(WiFi_Rev.r_buf, respond) != NULL)//����յ���Ҫ�Ļظ�
			{
				WiFi_Rev.r_buf[WiFi_Rev.r_len] = '\0';
				printf("%s\r\n", WiFi_Rev.r_buf);
				WiFi_Rev.r_len = 0;
				memset(WiFi_Rev.r_buf, 0, sizeof(WiFi_Rev.r_buf));//�������
				return 0;
			}
		}
	}
}

/**********************************************************
*�������ܣ�ģ�������ȵ�
*��    �����ȵ�����ssid   �ȵ�����pwd
*�� �� ֵ��0 �ɹ� ����ʧ��
*��    ע��
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
*�������ܣ�����ʱ����
*��    ����������
*�� �� ֵ��0 �ɹ� ����ʧ��
*��    ע��ClientID �еĶ���ǰ���һ��\
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
*�������ܣ�����MQTT����
*��    ����ClientID    Username   Password
*�� �� ֵ��0 �ɹ� ����ʧ��
*��    ע��ClientID �еĶ���ǰ���һ��\
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
*�������ܣ���������
*��    ����None
*�� �� ֵ��0 �ɹ� ����ʧ��
*��    ע��
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
*�������ܣ�����
*��    ����None
*�� �� ֵ��None
*��    ע��
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
*�������ܣ�ȡ������
*��    ����None
*�� �� ֵ��None
*��    ע��
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
*�������ܣ�������Ϣ
*��    ����None
*�� �� ֵ��None
*��    ע��
***********************************************************/
void WiFi_Issue(void)
{

}



/**********************************************************
*�������ܣ����ݰ�����
*��    ����None
*�� �� ֵ��None
*��    ע��
***********************************************************/
void Package_Analysis(void)
{


}









