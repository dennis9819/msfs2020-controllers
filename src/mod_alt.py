from fsuipc import FSUIPC
import serial
import time
from SimConnect import *

def current_milli_time():
    return round(time.time() * 1000)

def mod_alt_load(controllerDevice):
    sm = SimConnect()
    ae = AircraftEvents(sm)
    last_flaps = 0
    last_change_a4 = 0
    with FSUIPC() as fsuipc:
        controllerDevice.serial.write(b'INIT\n')
        prepared = fsuipc.prepare_data([
            (0x3324, "d"),
            (0x3324, "d")
        ], True)
        time.sleep(22)
        while True:
            alt1,alt2 = prepared.read()

            command_str = "S ALT %i\n"%(alt1)
            print(command_str)
            if alt1 > 13000:
                continue
            controllerDevice.serial.write(str.encode(command_str))
            time.sleep(0.5)
            