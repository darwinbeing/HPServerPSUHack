#! /usr/bin/python
# -*- coding: utf-8 -*-
#

# pip install intelhex
# CRC16 Hex File 0x2000 - 0x9ffe, BIN 0x4000 - 0x13ffc
# CRC16 Store location 0x9ffe: 8d 4f

from intelhex import IntelHex

ih = IntelHex()
ih.fromfile("11F/DSPIC33FJ64GS608.hex", format="hex")

START_ADDR=0x2000 * 2
END_ADDR=0xa000 * 2
bin_buf = ih.tobinarray(start=0, end=END_ADDR-1)

def hexdump(buf, start_addr=0, width=16):
    for i in range(0, len(buf), width):
        chunk = buf[i:i+width]

        hex_part = " ".join(f"{b:02X}" for b in chunk)
        ascii_part = "".join(chr(b) if 32 <= b <= 126 else "." for b in chunk)

        print(f"{start_addr + i:08X}  {hex_part:<{width*3}}  |{ascii_part}|")


# print(type(bin_buf), len(bin_buf))
hexdump(bin_buf, start_addr=0)

buf_sub = bin_buf[START_ADDR:END_ADDR-4]

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

CRC_STORE_ADDR = END_ADDR-4

crc_low = crc & 0xFF
crc_high = (crc >> 8) & 0xFF

ih[CRC_STORE_ADDR] = crc_low
ih[CRC_STORE_ADDR + 1] = crc_high

print(f"Write CRC16 to HEX:")
print(f"  HEX 0x{CRC_STORE_ADDR:06X} = {crc_low:02X}")
print(f"  HEX 0x{CRC_STORE_ADDR+1:06X} = {crc_high:02X}")

out_hex = "11F/DSPIC33FJ64GS608_crc.hex"
ih.tofile(out_hex, format="hex")

print(f"Saved new HEX file: {out_hex}")
