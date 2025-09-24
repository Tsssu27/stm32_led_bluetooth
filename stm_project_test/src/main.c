// #include "stm32f10x.h"
// #include "OLED.h"
// #include "DHT11.h"
// #include "Delay.h"

// u8 temperature, humidity;

// int main(void)
// {
//     OLED_Init();
//     DHT11_Init();
    
//     // 显示静态标题
//     OLED_ShowString(1, 1, "Temperature:");
//     OLED_ShowString(2, 1, "Humidity   :");

//     while(1)
//     {
//         if(DHT11_Read_Data(&temperature, &humidity) == 0)
//         {
//             // 显示温度值
//             OLED_ShowNum(1, 12, temperature, 3);
//             OLED_ShowString(1, 15, "C");
            
//             // 显示湿度值
//             OLED_ShowNum(2, 12, humidity, 3);
//             OLED_ShowString(2, 15, "%");
            
//             // 显示状态
//             OLED_ShowString(3, 1, "Status: OK      ");
//         }
//         else
//         {
//             // 显示错误状态
//             OLED_ShowString(3, 1, "Status: Error   ");
//         }
        
//         Delay_ms(2000);
//     }
// }

#include "stm32f10x.h"
#include "DHT11.h"
#include "Delay.h"
#include "serial.h"  // 添加串口头文件

u8 temperature, humidity;

int main(void)
{
    // 初始化串口和DHT11
    Serial_Init();
    DHT11_Init();
    
    // 串口发送启动信息
    Serial_SendString("DHT11 Temperature & Humidity Monitor Started\r\n");
    Serial_SendString("============================================\r\n");

    while(1)
    {
        if(DHT11_Read_Data(&temperature, &humidity) == 0)
        {
            // 通过串口发送温湿度数据
            Serial_Printf("Temperature: %dC, Humidity: %d%%\r\n", temperature, humidity);
            Serial_SendString("Status: OK\r\n");
        }
        else
        {
            // 发送错误信息
            Serial_SendString("Status: Error - Failed to read DHT11 data\r\n");
        }
        
        // 添加分隔线，便于阅读
        Serial_SendString("--------------------------------------------\r\n");
        
        Delay_ms(2000);  // 每2秒发送一次数据
    }
}