#ifndef _DHT11_H_
#define _DHT11_H_	
#include "stm32f10x.h"                  // Device header
#include "delay.h"

//��DHT11������ʼ�ź�
void DHT11_Start(void);
//���մӻ�����Ӧ�ź�
uint8_t DHT11_CheckResponse(void);
//��ȡһλ����λ
uint8_t DHT11_ReadBit(void);
//��ȡһ���ֽ�����
uint8_t DHT11_ReadByte(void);
//��ȡ�����ʪ��
uint8_t DHT11_ReadData(uint8_t* humidity, uint8_t* temperature);


#endif // DHT11_H