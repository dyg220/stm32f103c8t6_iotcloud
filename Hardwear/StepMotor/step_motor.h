#ifndef _STEP_MOTOR_H_
#define _STEP_MOTOR_H_
#include "stm32f10x.h"


void stepMotor_Init(void);
void stepMotor_Control(int spin, u8 dri);



#endif // !_STEP_MOTOR_H_
