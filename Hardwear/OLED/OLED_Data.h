#ifndef __OLED_DATA_H
#define __OLED_DATA_H

#include <stdint.h>

/*��ģ������Ԫ*/
typedef struct
{
	char Index[3];					//�����������ռ�Ϊ3�ֽ�
	uint8_t Data[32];				//��ģ����
} ChineseCell_t;

/*ASCII��ģ��������*/
extern const uint8_t OLED_F8x16[][16];
extern const uint8_t OLED_F6x8[][6];

/*������ģ��������*/
extern const ChineseCell_t OLED_CF16x16[];

/*ͼ����������*/
extern const uint8_t Diode[];
/*��������ĸ�ʽ�������λ�ü����µ�ͼ����������*/
//...

#endif

