import smbus

# I2C channel 1 is connected to the GPIO pins
channel = 1

#  MCP4725 defaults to address 0x60
address = 0x11


# Initialize I2C (SMBus)
bus = smbus.SMBus(channel)

def StringToBytes(val):
        retVal = []
        for c in val:
                retVal.append(ord(c))
        return retVal


def writeData(value):
        byteValue = StringToBytes(value)
        bus.write_i2c_block_data(address,0x00,byteValue)
        return -1

writeData("hello")