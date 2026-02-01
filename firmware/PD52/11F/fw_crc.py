#! /usr/bin/python
# -*- coding: utf-8 -*-
#

from intelhex import IntelHex

ih = IntelHex()
ih.fromfile("DSPIC33FJ64GS608_Orig.hex", format="hex")

bin_buf = ih.tobinarray(start=0, end=0x13fff)

def hexdump(buf, start_addr=0, width=16):
    for i in range(0, len(buf), width):
        chunk = buf[i:i+width]

        hex_part = " ".join(f"{b:02X}" for b in chunk)
        ascii_part = "".join(chr(b) if 32 <= b <= 126 else "." for b in chunk)

        print(f"{start_addr + i:08X}  {hex_part:<{width*3}}  |{ascii_part}|")


# print(type(bin_buf), len(bin_buf))
# hexdump(bin_buf, start_addr=0)

start = 0x4000
length = 0x13ffc-0x4000

buf_sub = bin_buf[start:start+length]

# Generate standard 256-entry CRC16 table (polynomial 0xA001)
def make_crc16_table():
    table = []
    for i in range(256):
        crc = i
        for _ in range(8):
            if crc & 1:
                crc = (crc >> 1) ^ 0xA001
            else:
                crc >>= 1
        table.append(crc)
    return table

CRC16_TABLE = make_crc16_table()
# for i in range(0, 256, 8):
#     print(", ".join(f"0x{v:04X}" for v in CRC16_TABLE[i:i+8]))

def crc16(data: bytes, init_crc: int = 0x0) -> int:
    crc = init_crc
    for byte in data:
        index = (byte ^ (crc & 0xFF))  # XOR input byte with low byte of CRC
        table_value = CRC16_TABLE[index]
        crc = (crc >> 8) ^ table_value
    return crc & 0xFFFF


crc = crc16(buf_sub)
print("CRC: " + hex(crc))
