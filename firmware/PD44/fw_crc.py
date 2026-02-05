#! /usr/bin/python
# -*- coding: utf-8 -*-
#

# pip install intelhex

from intelhex import IntelHex

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

def patch_crc16_bin_if_changed(
    ih: IntelHex,
    start_addr: int,
    end_addr: int,
    init_crc: int = 0x0000,
    verbose: bool = True
):
    crc_store_addr = end_addr - 4

    bin_buf = ih.tobinarray(start=0, end=end_addr - 1)

    old_crc = bin_buf[crc_store_addr] | (bin_buf[crc_store_addr + 1] << 8)

    data = bin_buf[start_addr:crc_store_addr]
    new_crc = crc16_ibm(data, init_crc)

    changed = (old_crc != new_crc)

    if verbose:
        print(f"[CRC]")
        print(f"  Range   : 0x{start_addr:06X} - 0x{crc_store_addr-1:06X}")
        print(f"  Old     : 0x{old_crc:04X}")
        print(f"  New     : 0x{new_crc:04X}")
        print(f"  Changed : {changed}")

    if changed:
        ih[crc_store_addr] = new_crc & 0xFF
        ih[crc_store_addr + 1] = (new_crc >> 8) & 0xFF

    return {
        "start": start_addr,
        "end": end_addr,
        "old_crc": old_crc,
        "new_crc": new_crc,
        "changed": changed,
    }

def patch_multiple_crc_regions(ih: IntelHex, regions, verbose=True):
    """
    regions: list of dicts
    [
        { "start": 0x4000, "end": 0x13FFC },
        { "start": 0x2000, "end": 0x9FFC },
    ]
    """

    results = []
    any_changed = False

    for i, r in enumerate(regions, 1):
        if verbose:
            print(f"\n=== CRC REGION #{i} ===")

        res = patch_crc16_bin_if_changed(
            ih,
            start_addr=r["start"],
            end_addr=r["end"],
            init_crc=r.get("init", 0x0000),
            verbose=verbose
        )

        results.append(res)
        any_changed |= res["changed"]

    return results, any_changed

ih = IntelHex()
ih.fromfile("05F/DSPIC33FJ64GS606.hex", format="hex")

regions = [
    {
        "start": 0x0,
        "end":   0x1000 * 2,
    },
    {
        "start": 0x1000 * 2,
        "end":   0x5c00 * 2,
    },
    {
        "start": 0x5c00 * 2,
        "end":   0xa800 * 2,
    },
]

results, changed = patch_multiple_crc_regions(ih, regions)

if changed:
    ih.tofile("05F/DSPIC33FJ64GS606_crc.hex", format="hex")
    print("\nHEX updated and saved")
else:
    print("\nAll CRCs already valid, no update needed")
