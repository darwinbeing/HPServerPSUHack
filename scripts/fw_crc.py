#! /usr/bin/python
# -*- coding: utf-8 -*-
#


file_path = '/Users/Thomas/Developer/HP-HSTNS-PLXX/HPServerPSUHack/firmware/stm32f100/STM32F100RBT6B.bin'
start = 0x4000
length = 0xbffc

custom_crc_table = {}
def generate_crc32_table(_poly):

    global custom_crc_table

    for i in range(256):
        c = i << 24

        for j in range(8):
            c = (c << 1) ^ _poly if (c & 0x80000000) else c << 1

        custom_crc_table[i] = c & 0xffffffff

def crc32_stm(bytes_arr):

    length = len(bytes_arr)
    crc = 0xffffffff

    k = 0
    while length >= 4:

        v = ((bytes_arr[k] << 24) & 0xFF000000) | ((bytes_arr[k+1] << 16) & 0xFF0000) | \
        ((bytes_arr[k+2] << 8) & 0xFF00) | (bytes_arr[k+3] & 0xFF)

        crc = ((crc << 8) & 0xffffffff) ^ custom_crc_table[0xFF & ((crc >> 24) ^ v)]
        crc = ((crc << 8) & 0xffffffff) ^ custom_crc_table[0xFF & ((crc >> 24) ^ (v >> 8))]
        crc = ((crc << 8) & 0xffffffff) ^ custom_crc_table[0xFF & ((crc >> 24) ^ (v >> 16))]
        crc = ((crc << 8) & 0xffffffff) ^ custom_crc_table[0xFF & ((crc >> 24) ^ (v >> 24))]

        k += 4
        length -= 4

    if length > 0:
        v = 0

        for i in range(length):
            v |= (bytes_arr[k+i] << 24-i*8)

        if length == 1:
            v &= 0xFF000000

        elif length == 2:
            v &= 0xFFFF0000

        elif length == 3:
            v &= 0xFFFFFF00

        crc = (( crc << 8 ) & 0xffffffff) ^ custom_crc_table[0xFF & ( (crc >> 24) ^ (v ) )];
        crc = (( crc << 8 ) & 0xffffffff) ^ custom_crc_table[0xFF & ( (crc >> 24) ^ (v >> 8) )];
        crc = (( crc << 8 ) & 0xffffffff) ^ custom_crc_table[0xFF & ( (crc >> 24) ^ (v >> 16) )];
        crc = (( crc << 8 ) & 0xffffffff) ^ custom_crc_table[0xFF & ( (crc >> 24) ^ (v >> 24) )];

    return crc


def calculate_crc(file_path, start, length):
    crc = 0xFFFFFFFF
    with open(file_path, 'rb') as file:
        file.seek(start)
        data = file.read(length)

    return crc32_stm(data)

poly = 0x04C11DB7
crc_loc = 0xfffc

generate_crc32_table(poly)
crc_stm = calculate_crc(file_path, start, length)
print("CRC: " + hex(crc_stm))

with open(file_path, 'rb') as file:
    file.seek(0xfffc)
    data = file.read(4)
    value = int.from_bytes(data, byteorder='little')
    # print("CRC: " + hex(value))

with open(file_path, 'r+b') as file:
    file.seek(0xfffc)
    data_bytes = crc_stm.to_bytes(4, byteorder='little')
    file.write(data_bytes)
