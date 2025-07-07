#include "wifi.h"
#include "usart2.h"

/****************************************************
 * ��������WiFi_Init
 * �������ܣ�WiFi��ʼ��
 * ��    ����None
 * �� �� ֵ��None
 ****************************************************/
void WiFi_Init(void)
{
	// ��ʼ��WiFiģ��
	// ���磬���ò����ʡ�����GPIO��
	USART2_Config(115200); // ����USART2������Ϊ115200
	// ������ʼ������...

	// ����ATָ�����WiFiģ���Ƿ���������
	//WiFi_SendAT("AT\r\n", "OK", 1000);
	WiFi_SendAT("AT+CWMODE=1\r\n", "OK", 1000);
	WiFi_SendAT("AT+CWJAP=\"tatt8990\",\"22336699\"\r\n", "OK", 2000);
	WiFi_SendAT("AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"\r\n\"", "OK", 5000);
	WiFi_SendAT("AT+MQTTUSERCFG=0,1,\"123|securemode=3\\,signmethod =hmacsha1|\",\"SamarHomeV1.0&a1sQGf30de7\",\"EC8EFA2B87BE190DA84F0895A2D2ECDD51E0CFB4\",0,0,\"\"\r\n", "OK", 10000);
	WiFi_SendAT("AT+MQTTCONN=0,\"a1sQGf30de7.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883,1\r\n", "OK", 2000);



}

/****************************************************
 * �� �� ����WiFi_SendAT
 * �������ܣ�WiFi����ATָ��
 * ��    ����ATCmd��ATָ�� respond����������Ӧ  timeout����ʱʱ��
 * �� �� ֵ��None
 ****************************************************/
void WiFi_SendAT(char* AT, char* respond, u32 TimeOut)
{
	USART2_SendStr(AT); // ����ATָ��
	delay_ms(100);      // �ȴ�ģ����Ӧ
	// �ȴ���Ӧ
	u32 TimeCnt = 0;
	while (TimeCnt < TimeOut)
	{
		if (WiFi_Rev.r_Flag) // ������յ�����
		{
			if (strstr((char*)WiFi_Rev.r_buf, respond) != NULL) // ����Ƿ������������Ӧ
			{
				printf("Response received: %s\n", WiFi_Rev.r_buf);
				WiFi_Rev.r_Flag = 0; // ���ñ�־
				return;               // �ɹ�������Ӧ
			}
			WiFi_Rev.r_Flag = 0; // ���ñ�־
		}
		TimeCnt++;   // ��������ʱ��
		delay_ms(2000); // �ȴ�һ��ʱ���ټ��
	}
	printf("No response or timeout.\n"); // ��ʱ��δ�յ���Ӧ
}
