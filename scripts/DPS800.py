from machine import I2C, Pin
import time

class DPS800:

    def __init__(self, i2c_id=0, sda=0, scl=1, address_offset=7):
        self.i2c = I2C(i2c_id, sda=Pin(sda), scl=Pin(scl), freq=5000)
        self.address = 0x58 + address_offset
        print("Scanning I2C bus...")

#         devices = self.i2c.scan()
#         if devices:
#             print("Found devices:")
#             for addr in devices:
#                 print(" - 0x{:02X}".format(addr))
#         else:
#             print("No I2C devices found.")

        # print(" - 0x{:02X}".format(devices[1]))

    def checksum(self, *data):
        total = (self.address << 1)
        for b in data:
            total += b
        return (-total) & 0xFF

    def read_register(self, cmd, reg):

        cs = self.checksum(cmd, reg)

        # print(" - 0x{:02X} 0x{:02X} 0x{:02X} 0x{:02X}".format(self.address, cmd , reg, cs))
        ret = self.i2c.writeto(self.address, bytes([cmd, reg, cs]))
        data = self.i2c.readfrom(self.address, 3)
        # print("RX:", " ".join("{:02X}".format(x) for x in data))
        total = sum(data) & 0xFF
        #if total != 0:
        #    raise Exception("Checksum error")

        value = data[0] | (data[1] << 8)
        return value


    def read_voltage(self):
        #0x10 0x20
        raw = self.read_register(0x00, 0x20)
        return raw / 256.0

    def read_current(self):
        #0x11 0x12 0x21 0x22
        raw = self.read_register(0x00, 0x12)
        return raw / 12.0



ps = DPS800(i2c_id=0, sda=0, scl=1)

# while True:
#     print(" voltage: {:.2f}".format(ps.read_voltage()))
#     #print(" current: {:.2f}".format(ps.read_current()))

#     time.sleep(1)

ranges = [
    range(0x01, 0x02),
    range(0x10, 0x18),
    range(0x20, 0x25),
    range(0x32, 0x4A),
    range(0x60, 0x63),
    range(0xF0, 0xF7),
]

COLS = 16

while True:
    # print("\x1b[2J\x1b[H")  # clear screen



    for r in ranges:
        line = ""
        count = 0
        for addr in r:
            v = ps.read_register(0x00, addr)

            if v is not None:
                line += "0x{:02X}:0x{:04X}   ".format(addr, v)
                # line += "0x{:02X}: {:.2f}   ".format(addr, v / 256)
                count += 1

                if count % COLS == 0:
                    print(line)
                    line = ""

        if line:   # print remaining
            print(line)

        time.sleep(0.5)
