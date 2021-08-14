
import serial
import time
import random

def current_milli_time():
    return round(time.time() * 1000)

def mod_alt_load(controllerDevice):
    alt1 = 0
    dir = 1
    controllerDevice.serial.write(b'INIT\n')
    time.sleep(22)
    while True:
        dir_ch = random.randrange(0,100)
        if (dir_ch > 95):#
            if dir == -1:
                dir = 1
            else:
                dir = -1
        alt1+= (random.randrange(0,5) * dir)
        if alt1 < 0:
            alt1 = 0
            if dir == -1:
                dir = 1
   
        command_str = "S ALT %i\n"%(alt1)
        print(command_str)
        if alt1 > 13000:
            continue
        controllerDevice.serial.write(str.encode(command_str))
        time.sleep(1/60)
            