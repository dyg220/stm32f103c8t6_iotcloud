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
    WiFi_SendAT("AT\r\n", "OK", 1000);
}

/****************************************************
 * �� �� ����WiFi_SendAT
 * �������ܣ�WiFi����ATָ��
 * ��    ����ATCmd��ATָ�� respond����������Ӧ  timeout����ʱʱ��
 * �� �� ֵ��None
 ****************************************************/
void WiFi_SendAT(char *AT, char *respond, u32 TimeOut)
{
    USART2_SendStr(AT); // ����ATָ��
    delay_ms(100);      // �ȴ�ģ����Ӧ
    // �ȴ���Ӧ
    u32 TimeCnt = 0;
    while (TimeCnt < TimeOut)
    {
        if (WiFi_INFO.r_Flag) // ������յ�����
        {
            if (strstr((char *)WiFi_INFO.r_buf, respond) != NULL) // ����Ƿ������������Ӧ
            {
                printf("Response received: %s\n", WiFi_INFO.r_buf);
                WiFi_INFO.r_Flag = 0; // ���ñ�־
                return;               // �ɹ�������Ӧ
            }
            WiFi_INFO.r_Flag = 0; // ���ñ�־
        }
        TimeCnt++;   // ��������ʱ��
        delay_ms(2); // �ȴ�һ��ʱ���ټ��
    }
    printf("No response or timeout.\n"); // ��ʱ��δ�յ���Ӧ
}
