#ifndef _WIFI_H_
#define _WIFI_H_

#include "stm32f10x.h"                  // Device header
#include "usart2.h"
#include "delay.h"
#include "string.h"
#include "stdlib.h"
#include "relay.h"
#include "led.h"

void WiFi_Init(void);
u8 WiFi_SendAT(char *AT,char *respond,u16 TimeOut);
u8 WiFi_ConnectAP(char *ssid,char *pwd);
u8 WiFi_SetTime(char* Server);
u8 WiFi_SetMQTT(char *ClientID,char *Username,char *Password);
u8 WiFi_SendDomain(char*IP,u16 port);
u8 WiFi_Subscription(char *SUB);
u8 WiFi_NOSubscription(char *SUB);

#endif
