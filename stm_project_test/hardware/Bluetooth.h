#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "stm32f10x.h"

void Bluetooth_Init(void);
void Bluetooth_Send(uint8_t data);
void Bluetooth_ReceiveHandler(uint8_t data);  // 接收一个字节时调用
void Bluetooth_Process(void);
void Bluetooth_ExecuteCommand(uint8_t cmd, uint8_t seq, uint8_t *data, uint8_t len);
#endif
