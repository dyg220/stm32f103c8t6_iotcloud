#ifndef _DHT11_H_
#define _DHT11_H_	
#include "stm32f10x.h"                  // Device header
#include "delay.h"

//向DHT11发送起始信号
void DHT11_Start(void);
//接收从机的响应信号
uint8_t DHT11_CheckResponse(void);
//读取一位数据位
uint8_t DHT11_ReadBit(void);
//读取一个字节数据
uint8_t DHT11_ReadByte(void);
//获取外界温湿度
uint8_t DHT11_ReadData(uint8_t* humidity, uint8_t* temperature);


#endif // DHT11_H