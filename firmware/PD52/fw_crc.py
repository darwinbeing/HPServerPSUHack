#! /usr/bin/python
# -*- coding: utf-8 -*-
#

# pip install intelhex

from intelhex import IntelHex

ih = IntelHex()
ih.fromfile("11F/DSPIC33FJ64GS608.hex", format="hex")

def hexdump(buf, start_addr=0, width=16):
    for i in range(0, len(buf), width):
        chunk = buf[i:i+width]

        hex_part = " ".join(f"{b:02X}" for b in chunk)
        ascii_part = "".join(chr(b) if 32 <= b <= 126 else "." for b in chunk)

        print(f"{start_addr + i:08X}  {hex_part:<{width*3}}  |{ascii_part}|")

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

def crc16_ibm(data: bytes, init_crc: int = 0x0) -> int:
    crc = init_crc
    for byte in data:
        index = (byte ^ (crc & 0xFF))  # XOR input byte with low byte of CRC
        table_value = CRC16_TABLE[index]
        crc = (crc >> 8) ^ table_value
    return crc & 0xFFFF

def patch_crc16(
    ih: IntelHex,
    start_addr: int,
    end_addr: int,
    init_crc: int = 0x0000,
    verbose: bool = True
) -> int:
    """
    start_addr / end_addr are BIN addresses
    CRC is stored at end_addr - 4 (little endian)
    CRC range: [start_addr, end_addr - 4)
    """

    crc_store_addr = end_addr - 4

    bin_buf = ih.tobinarray(start=0, end=end_addr - 1)

    data = bin_buf[start_addr:crc_store_addr]

    crc = crc16_ibm(data, init_crc)

    ih[crc_store_addr] = crc & 0xFF
    ih[crc_store_addr + 1] = (crc >> 8) & 0xFF

    if verbose:
        print(f"CRC16-IBM patched")
        print(f"  CRC range : 0x{start_addr:06X} - 0x{crc_store_addr-1:06X}")
        print(f"  CRC value : 0x{crc:04X}")
        print(f"  Store @  : 0x{crc_store_addr:06X}")

    return crc

crc = patch_crc16(
    ih,
    start_addr=0,
    end_addr=0x2000 * 2
    )

crc = patch_crc16(
    ih,
    start_addr=0x2000 * 2,
    end_addr=0xa000 * 2
    )

out_hex = "11F/DSPIC33FJ64GS608_crc.hex"
ih.tofile(out_hex, format="hex")

print(f"Saved new HEX file: {out_hex}")
