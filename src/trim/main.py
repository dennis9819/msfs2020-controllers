from fsuipc import FSUIPC
import math
import serial
import time

import vgamepad as vg

gamepad = vg.VX360Gamepad()

ser = serial.Serial('COM6',115200)

with FSUIPC() as fsuipc:
    prepared = fsuipc.prepare_data([
        (0x2EA0, "f"),
        (0x2EA0, "f")
    ], True)
    time.sleep(4)
    while True:
        trim,trim2 = prepared.read()

        calcTrim = round(((round((trim),3) +0.34) * 147),0)
        converted_num = "S1 %s\n" % int(calcTrim)

        ser.write(bytes(converted_num, 'ascii'))

        while ser.in_waiting:  # Or: while ser.inWaiting():
            val = ser.readline()
           # print(val)
            if val == b'UP\r\n':
                if trim > -0.34:
                    fsuipc.write([(0x2EA0, "f", trim - 0.005)])

            if val == b'DOWN\r\n':
                if trim < 0.34:
                    fsuipc.write([(0x2EA0, "f", trim + 0.005)])




