#include "stm32f10x.h"

#include "led.h"

// 假设 LED0 -> PA0, LED1 -> PA1
void LED_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;   // 推挽输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 默认熄灭
    GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1);
}

void LED0_On(void) {
    GPIO_ResetBits(GPIOA, GPIO_Pin_0);   // 低电平亮
}

void LED0_Off(void) {
    GPIO_SetBits(GPIOA, GPIO_Pin_0);     // 高电平灭
}

void LED1_On(void) {
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

void LED1_Off(void) {
    GPIO_SetBits(GPIOA, GPIO_Pin_1);
}
