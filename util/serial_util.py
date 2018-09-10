import serial, io, os, sys
from time import sleep
from serial.tools.list_ports import comports

data_len = 128
head_byte = {
    128: b'\x01',
    1024: b'\x02',
}
baudrate = 115200
delay = lambda: sleep(0.0005)

def get_crc(b):
    crc = 0
    for i in b:
        crc ^= i << 8
        for _ in range(8):
            if crc & 0x8000:
                crc = (crc << 1) ^ 0x1021
            else:
                crc <<= 1
        crc &= 0xffff
    return bytes([crc >> 8, crc & 0xff])

def send_xmodem(file_name, ser):
    while True:
        print(ser.read())
        if ser.read() != b'C':
            print('waiting for transmission')
        print('start')
        with open(file_name, 'rb') as f:
            size = os.path.getsize(file_name)
            for i in range(0, size, data_len):
                # send header
                ser.write(head_byte[data_len])
                print('header')
                delay()
                # send package number
                pack_num = int(i / data_len) + 1
                ser.write(bytes([pack_num & 0xff]))
                print('pn 0', bytes([pack_num & 0xff]))
                delay()
                ser.write(bytes([~(pack_num & 0xff) & 0xff]))
                print('pn 1', bytes([~(pack_num & 0xff) & 0xff]))
                delay()
                # get current data
                data = f.read(data_len)
                if len(data) < data_len:
                    iob = io.BytesIO()
                    iob.write(data)
                    iob.write(b'\x1a' * (data_len - len(data)))
                    data = iob.getvalue()
                # send data
                for b in data:
                    ser.write(b)
                    print('data', b, ser.read())
                    delay()
                # send CRC
                crc = get_crc(data)
                ser.write(crc[0])
                delay()
                ser.write(crc[1])
                delay()
                # wait ACK
                while ser.read() != b'\x06':
                    print('wait ACK 0')
            # send EOT
            ser.write(b'\x04')
            delay()
            # wait ACK
            while ser.read() != b'\x06':
                print('wait ACK 1')
            # end of transmission
            print('EOT')
            break

def get_word(num):
    byte_list = []
    for _ in range(4):
        byte_list.append(int(num % 256))
        num /= 256
    return bytes(byte_list)

def send_uart(file_name, ser):
    with open(file_name, 'rb') as f:
        size = os.path.getsize(file_name)
        ser.write(get_word(size))
        delay()
        for _ in range(0, size, 4):
            ser.write(f.read(4))
            delay()

if __name__ == '__main__':
    args = sys.argv
    args.pop(0)

    if len(args) < 2:
        for i in comports():
            print(i.device)
    else:
        print(args[0])
        ser = serial.Serial(args[0], baudrate, timeout=1)
        send_uart(args[1], ser)
