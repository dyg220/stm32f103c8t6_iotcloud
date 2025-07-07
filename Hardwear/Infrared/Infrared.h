#ifndef _INFRARED_H_
#define _INFRARED_H_

#include "stm32f10x.h"                  // Device header
#include "tim2.h"
#include "stdio.h"

typedef struct{
	u16 TimeCnt[128];//电平时间
	u8 EdgeCnt;//边沿数
	u8 CaptureOver;//捕获完成标志
	u8 NECBit[64];//红外编码位
	u8 NECCode[10];//红外编码
	u8 ContorlCode;//控制码
	
}INFRARED_INFO;//红外结构体

extern INFRARED_INFO Infrared_info_t;


void Infrared_Init(void);
u8 Infrared_AnalysisNECBit(u8 type);
u8 Infrared_AnalysisNECCode(u8 type);




#endif
