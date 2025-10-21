#include "stm32f10x.h"
#include "led.h"
#include "Bluetooth.h"
#include "pwm.h"
#include "motor.h"


int main(void)
{
    PWM_GPIO_TIM_Init();
    Motor_Init();
    Bluetooth_Init();

    while(1)
    {
        Bluetooth_Process();
    }
}
