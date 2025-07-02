#include "oled.h"
#include "spi2.h"

void OLED_Pin_Init(void)
{
	// ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	// ����GPIO
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
�������ܣ�OLEDд����
��    ����None
�� �� ֵ��None
��    ע��
*****************************************************/
void OLED_WriteCommand(u8 Command)
{
	OLED_DC_L;              // ����DC,��ʾд����
	OLED_CS_L;              // ����CS,��ʾͨ�ſ�ʼ
	SPI2_Transmit(Command); // д��ָ������
	OLED_CS_H;              // ����CS,��ʾͨ�Ž���
}

/****************************************************
�������ܣ�OLEDд����
��    ����None
�� �� ֵ��None
��    ע��
*****************************************************/
void OLED_WriteData(u8 Data)
{
	OLED_DC_H;           // ����DC,��ʾд����
	OLED_CS_L;           // ����DC,��ʾͨ�ſ�ʼ
	SPI2_Transmit(Data); // д��ָ������
	OLED_CS_H;           // ����DC,��ʾͨ�Ž���
}

/****************************************************
�������ܣ�OLED��ʼ��
��    ����None
�� �� ֵ��None
��    ע��OLED��������---�����ṩ
*****************************************************/
void OLED_Init(void)
{
	OLED_Pin_Init(); // ���ų�ʼ��
	OLED_RES_H;      // ��λ
	delay_ms(100);
	OLED_RES_L; // ��λ
	delay_ms(100);
	OLED_RES_H;

	/*д��һϵ�е������OLED���г�ʼ������*/
	OLED_WriteCommand(0xAE); // ������ʾ����/�رգ�0xAE�رգ�0xAF����

	OLED_WriteCommand(0xD5); // ������ʾʱ�ӷ�Ƶ��/����Ƶ��
	OLED_WriteCommand(0x80); // 0x00~0xFF

	OLED_WriteCommand(0xA8); // ���ö�·������
	OLED_WriteCommand(0x3F); // 0x0E~0x3F

	OLED_WriteCommand(0xD3); // ������ʾƫ��
	OLED_WriteCommand(0x00); // 0x00~0x7F

	OLED_WriteCommand(0x40); // ������ʾ��ʼ�У�0x40~0x7F

	OLED_WriteCommand(0xA1); // �������ҷ���0xA1������0xA0���ҷ���

	OLED_WriteCommand(0xC8); // �������·���0xC8������0xC0���·���

	OLED_WriteCommand(0xDA); // ����COM����Ӳ������
	OLED_WriteCommand(0x12);

	OLED_WriteCommand(0x81); // ���öԱȶ�
	OLED_WriteCommand(0xCF); // 0x00~0xFF

	OLED_WriteCommand(0xD9); // ����Ԥ�������
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB); // ����VCOMHȡ��ѡ�񼶱�
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4); // ����������ʾ��/�ر�

	OLED_WriteCommand(0xA6); // ��������/��ɫ��ʾ��0xA6������0xA7��ɫ

	OLED_WriteCommand(0x8D); // ���ó���
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF); // ������ʾ

	OLED_Clear(); // ����
}

// ���ù��(��ʼλ��)
void OLED_setCursor(u8 X, u8 Page)
{
	if (Page > 7 || X > 127) return;           // ��Χ
	OLED_WriteCommand(0xB0 | Page);            // ����ҳ��ַ
	OLED_WriteCommand(0x00 | (X & 0x0F));      // ����x�еĵ���λ
	OLED_WriteCommand(0x10 | (X & 0xF0) >> 4); // ����x�еĸ���λ
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
