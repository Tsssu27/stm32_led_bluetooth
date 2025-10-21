import tkinter as tk
import serial

ser = serial.Serial("COM9", 9600, timeout=1)

def send_command(cmd, data=[]):
    seq = 1
    length = len(data)
    packet = [0xAA, cmd, seq, length] + data
    checksum = sum(packet[1:]) & 0xFF
    packet.append(checksum)
    ser.write(bytearray(packet))
    print("发送:", packet)

def forward():
    send_command(0x01)

def backward():
    send_command(0x02)

def stop():
    send_command(0x03)

def set_speed(val):
    speed = int(val)
    send_command(0x05, [speed])

root = tk.Tk()
root.title("小车蓝牙控制")

tk.Button(root, text="前进", command=forward).pack()
tk.Button(root, text="后退", command=backward).pack()
tk.Button(root, text="停止", command=stop).pack()
tk.Scale(root, label="速度", from_=0, to=100, orient=tk.HORIZONTAL, command=set_speed).pack()

root.mainloop()
