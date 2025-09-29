import Port_Monitor
import tkinter as tk
import threading
import serial

# 串口配置
ser = serial.Serial("COM9", 9600, timeout=1)

# ================= 串口收数据 =================
def listen_serial():
    print("监听线程已启动...")
    while True:
        data = ser.read(50)
        if data:
            try:
                hex_str = data.hex(" ").upper()
                txt_str = data.decode(errors="ignore")
                text_box.insert(tk.END, f"收到 HEX: {hex_str}\n")
                text_box.insert(tk.END, f"收到 TXT: {txt_str}\n\n")
                text_box.see(tk.END)  # 自动滚动
            except Exception as e:
                print("解码错误:", e)

thread = threading.Thread(target=listen_serial, daemon=True)
thread.start()

# ================= 发送函数 =================
def send_led0_on():
    cmd = bytes([0xAA, 0x01, 0x01, 0x00, 0x02])  # LED0 ON
    ser.write(cmd)
    text_box.insert(tk.END, "发送: LED0 ON\n")

def send_led0_off():
    cmd = bytes([0xAA, 0x02, 0x01, 0x00, 0x03])  # LED0 OFF
    ser.write(cmd)
    text_box.insert(tk.END, "发送: LED0 OFF\n")

def send_led1_on():
    cmd = bytes([0xAA, 0x03, 0x01, 0x00, 0x04])  # LED1 ON
    ser.write(cmd)
    text_box.insert(tk.END, "发送: LED1 ON\n")

def send_led1_off():
    cmd = bytes([0xAA, 0x04, 0x01, 0x00, 0x05])  # LED1 OFF
    ser.write(cmd)
    text_box.insert(tk.END, "发送: LED1 OFF\n")




# ================= GUI 界面 =================
root = tk.Tk()
root.title("蓝牙 LED 控制器")

# LED0 按钮
btn_led0_on = tk.Button(root, text="LED0 ON", command=send_led0_on, width=12)
btn_led0_on.grid(row=0, column=0, padx=10, pady=10)

btn_led0_off = tk.Button(root, text="LED0 OFF", command=send_led0_off, width=12)
btn_led0_off.grid(row=0, column=1, padx=10, pady=10)

# LED1 按钮
btn_led1_on = tk.Button(root, text="LED1 ON", command=send_led1_on, width=12)
btn_led1_on.grid(row=1, column=0, padx=10, pady=10)

btn_led1_off = tk.Button(root, text="LED1 OFF", command=send_led1_off, width=12)
btn_led1_off.grid(row=1, column=1, padx=10, pady=10)

# 输出框
text_box = tk.Text(root, height=15, width=60)
text_box.grid(row=2, column=0, columnspan=2, padx=10, pady=10)

root.mainloop()
