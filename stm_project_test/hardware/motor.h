#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"

void Motor_Init(void);
void Motor_RunForward(void);
void Motor_RunBackward(void);
void Motor_Stop(void);

#endif
