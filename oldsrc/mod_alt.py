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
    command_str = "S ALT %i\n"%(0)
    print(command_str)
    controllerDevice.serial.write(str.encode(command_str))
    command_str = "S BAR %i\n"%(2810)
    print(command_str)
    controllerDevice.serial.write(str.encode(command_str))
    with FSUIPC() as fsuipc:
        controllerDevice.serial.write(b'INIT\n')
        prepared = fsuipc.prepare_data([
            (0x3324, "d"),
            (0x3324, "d"),
            (0x0330, "h"),
        ], True)
        time.sleep(28)
        while True:
            alt1,alt2,baro = prepared.read()
            if alt1 > 13000:
                continue
            command_str = "S ALT %i\n"%(alt2 * 3.28084)
            #print(command_str)
            controllerDevice.serial.write(str.encode(command_str))
            command_str = "S BAR %i\n"%((baro /16)*0.02953 * 100)
            #print(command_str)
            controllerDevice.serial.write(str.encode(command_str))
            time.sleep(0.08)
            