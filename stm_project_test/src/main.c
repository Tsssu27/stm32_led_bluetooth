#include "stm32f10x.h"
#include "led.h"
#include "Bluetooth.h"

int main(void) {
    // 初始化 LED 和 蓝牙
    LED_Init();
    Bluetooth_Init();

    // 测试：上电先点亮 LED0，熄灭 LED1
    LED0_On();
    LED1_Off();

    while (1) {
       Bluetooth_Process(); // 处理蓝牙接收的数据
    }
}
