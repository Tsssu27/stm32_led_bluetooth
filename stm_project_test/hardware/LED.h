#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

void LED_Init(void);

void LED0_On(void);
void LED0_Off(void);

void LED1_On(void);
void LED1_Off(void);

uint8_t LED0_GetState(void);
uint8_t LED1_GetState(void);
#endif
