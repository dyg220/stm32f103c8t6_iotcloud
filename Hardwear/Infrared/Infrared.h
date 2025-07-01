#ifndef _INFRARED_H_
#define _INFRARED_H_

#include "stm32f10x.h"                  // Device header
#include "tim2.h"
#include <stdio.h>

typedef struct {
	u16 TimeCnt[128];//��ƽʱ��
	u8 EdgeCnt;		//������
	u8 CaptureOver;//������ɱ�־
	u8 NECBit[64];//�������λ

}INFRARED_INFO;//����ṹ��

extern INFRARED_INFO Infrared_info_t;


void Infrared_Init(void);
u8 Infrared_AnalysisNECBit(u8 type);




#endif
