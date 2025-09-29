#include "Bluetooth.h"
#include "serial.h"
#include "led.h"   // 假设你有 LED 控制函数


#define FRAME_HEADER 0xAA
#define FRAME_MAX_LEN 32

#define CMD_ACK 0xFE
#define CMD_NACK 0xFF
#define CMD_STATUS 0x10

#define ERR_CHECKSUM 0x01
#define ERR_LEN 0x02
#define ERR_UNKNOWN 0x03

static uint8_t rx_state = 0;
static uint8_t rx_buf[FRAME_MAX_LEN];
static uint8_t rx_len = 0;
static uint8_t data_len = 0;
static uint8_t cmd = 0;
static uint8_t seq = 0;

void Bluetooth_SendAck(uint8_t s);
void Bluetooth_SendNack(uint8_t s, uint8_t err);
void Bluetooth_ExecuteCommand(uint8_t cmd, uint8_t seq, uint8_t *data, uint8_t len);

// 初始化蓝牙（实际上就是初始化USART）
void Bluetooth_Init(void) {
    Serial_Init();  // 调用你usart.c里的初始化
}

// 发送一个字节
void Bluetooth_Send(uint8_t data) {
    Serial_SendByte(data);  // 调用usart.c里的发送
}

// 处理接收到的数据（命令解析）
void Bluetooth_Process(void){
    while(Serial_Available()){
        uint8_t byte = Serial_Read();

        switch(rx_state){

            case 0 :
                if( byte == FRAME_HEADER){// 收到包头
                    rx_state = 1;
                    rx_len = 0;//rxlen是数据区长度，不是整个包长度
                    
                    
                }
                break;

            case 1 :      // 收到命令字
                cmd = byte;
                rx_state = 2;
                rx_len = 0;
                break;

            case 2 :// 序列
                seq = byte;
                rx_state = 3;
                break;
            //为什么要这么多次执行rxlen=0？因为每次进入新的状态都要重新开始计数数据区长度
            //否则会出错，比如先收到命令字，再收到数据长度，然后数据区长度就会变成2，而不是1
            case 3 ://LEN
                data_len = byte;
                if(data_len > FRAME_MAX_LEN){
                    Serial_Printf("Len error (len=%d)\r\n", data_len);
                    Bluetooth_SendNack(seq, ERR_LEN);
                    rx_state = 0; // 长度错误，丢弃数据
                }else{
                    rx_len = 0;
                    if(data_len == 0){
                        rx_state = 5; // 没有数据区，直接进入校验和状态
                }else{
                        rx_state = 4; // 有数据区，进入数据接收状态
                    }
                }
                break;
         
            case 4 :// data
            {
                rx_buf[rx_len++] = byte;
                if(rx_len >= data_len){
                    rx_state = 5; // 数据接收完毕，进入校验和状态
                }
            }
                break;

            case 5 :// checksum
            {
                uint8_t checksum = 0;
                
                checksum = (uint8_t)(cmd + seq + data_len);

                for (uint8_t i = 0; i < data_len; i++) checksum += rx_buf[i];
                checksum &= 0xFF;

                if(checksum == byte){
                   Bluetooth_ExecuteCommand(cmd, seq, rx_buf, data_len);
                } else {
                    Serial_Printf("Checksum error (calc=0x%02X recv=0x%02X)\r\n", checksum, byte);
                    Bluetooth_SendNack(seq, ERR_CHECKSUM);
                }
                rx_state = 0;
            } break;

            default:
                rx_state = 0;
                break;

        }

    }
}

// ================== 新增：ACK/NACK 发送函数 ==================
void Bluetooth_SendAck(uint8_t s) {
   uint8_t buf[5];
    buf[0] = FRAME_HEADER;
    buf[1] = CMD_ACK;
    buf[2] = s;
    buf[3] = 0x00; // len = 0
    uint8_t chk = (uint8_t)(buf[1] + buf[2] + buf[3]);
    buf[4] = chk & 0xFF;
    Serial_SendArray(buf, 5);
}

void Bluetooth_SendNack(uint8_t s, uint8_t err) {
    uint8_t buf[6];
    buf[0] = FRAME_HEADER;
    buf[1] = CMD_NACK;
    buf[2] = s;
    buf[3] = 0x01; // len = 1
    buf[4] = err;
    uint8_t chk = (uint8_t)(buf[1] + buf[2] + buf[3] + buf[4]);
    buf[5] = chk & 0xFF;
    Serial_SendArray(buf, 6);
}

void Bluetooth_SendStatus(uint8_t s, uint8_t led0, uint8_t led1, uint8_t brightness) {
    uint8_t buf[7];
    buf[0] = FRAME_HEADER;
    buf[1] = CMD_STATUS;    // 状态上报命令字
    buf[2] = s;       // 序列号
    buf[3] = 0x02;    // 数据长度

    // 数据：LED 状态 + 亮度
    uint8_t led_status = 0;
    if (led0) led_status |= 0x01;
    if (led1) led_status |= 0x02;
    buf[4] = led_status;
    buf[5] = brightness;

    // 校验和
    uint8_t chk = buf[1] + buf[2] + buf[3] + buf[4] + buf[5];
    buf[6] = chk & 0xFF;

    Serial_SendArray(buf, 7);
}



void Bluetooth_ExecuteCommand(uint8_t cmd, uint8_t seq, uint8_t *data, uint8_t len) {
    switch (cmd) {
        case 0x01: // LED0 ON
            LED0_On();
            Serial_Printf("LED0 ON\r\n");
            Bluetooth_SendAck(seq);
            Bluetooth_SendStatus(seq,LED0_GetState(),LED1_GetState(),50); // 发送状态更新
            break;

        case 0x02: // LED0 OFF
            LED0_Off();
            Serial_Printf("LED0 OFF\r\n");
            Bluetooth_SendAck(seq);
            break;

        case 0x03: // LED1 ON
            LED1_On();
            Serial_Printf("LED1 ON\r\n");
            Bluetooth_SendAck(seq);
            break;

        case 0x04: // LED1 OFF
            LED1_Off();
            Serial_Printf("LED1 OFF\r\n");
            Bluetooth_SendAck(seq);
            break;

        case 0x05: // 亮度控制，data[0] = 0..100
            if (len >= 1) {
                uint8_t brightness = data[0];
                // TODO: PWM_SetBrightness(brightness);
                Serial_Printf("Set Brightness=%d\r\n", brightness);
                Bluetooth_SendAck(seq);
            } else {
                Bluetooth_SendNack(seq, ERR_LEN);
            }
            break;

        default:
            Serial_Printf("Unknown CMD: 0x%02X\r\n", cmd);
            Bluetooth_SendNack(seq, ERR_UNKNOWN);
            break;
    }
}