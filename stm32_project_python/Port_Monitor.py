import serial

# 打开串口（把 COM5 改成你自己的端口号）
ser = serial.Serial("COM9", 9600, timeout=1)

# 发送 LED0 ON 指令: AA 01 01 00 02
cmd = bytes([0xAA, 0x01, 0x01, 0x00, 0x02])
ser.write(cmd)

print("发送 LED0 ON 指令...")

# 读取 MCU 返回的数据
while True:
    data = ser.read(20)  # 最多读20字节
    if data:
        print("收到:", data.hex(" ").upper())
