#include "motor.h"

void Motor_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; // AIN1, AIN2, STBY
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_SetBits(GPIOA, GPIO_Pin_3); // STBY = 1，启动驱动芯片
}

void Motor_RunForward(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_1);   // AIN1 = 1
    GPIO_ResetBits(GPIOA, GPIO_Pin_2); // AIN2 = 0
}

void Motor_RunBackward(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_1); // AIN1 = 0
    GPIO_SetBits(GPIOA, GPIO_Pin_2);   // AIN2 = 1
}

void Motor_Stop(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2); // AIN1 = 0, AIN2 = 0
}
