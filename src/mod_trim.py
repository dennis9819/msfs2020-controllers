from fsuipc import FSUIPC
import serial
import time
from SimConnect import *

def current_milli_time():
    return round(time.time() * 1000)

def mod_load(controllerDevice):
    sm = SimConnect()
    ae = AircraftEvents(sm)
    last_flaps = 0
    last_change_a4 = 0
    with FSUIPC() as fsuipc:
        event_a1 = ae.find("GLARESHIELD_LIGHTS_SET") 
        event_a0 = ae.find("PEDESTRAL_LIGHTS_SET") 
        print(event_a0,event_a1)

        prepared = fsuipc.prepare_data([
            (0x2EA0, "f"),
            (0x0BDC, "l")
        ], True)
        time.sleep(4)
        while True:
            trim,flaps = prepared.read()

            calcTrim = round(((round((trim),3) +0.34) * 147),0)
            converted_num = "S1 %s\n" % int(calcTrim)

            controllerDevice.serial.write(bytes(converted_num, 'ascii'))

            while controllerDevice.serial.in_waiting:  # Or: while ser.inWaiting():
                val = controllerDevice.serial.readline()
                #print(val)
                if val == b'UP\r\n':
                    if trim > -0.34:
                        fsuipc.write([(0x2EA0, "f", trim - 0.005)])

                if val == b'DOWN\r\n':
                    if trim < 0.34:
                        fsuipc.write([(0x2EA0, "f", trim + 0.005)])

                if val[0:2] == b'A4':
                    value = int(val[3:])
                    
                    cur = 0
                    if value > 495:
                        cur = 10
                    if value > 535:
                        cur = 20
                    if value > 570:
                        cur = 30
                    if (cur != last_flaps):
                        if ( last_change_a4 < current_milli_time() - 150):
                            last_change_a4 = current_milli_time()
                            last_flaps = cur
                            
                            # 0BDC
                            #fsuipc.write([(0x0BDC, "l", last_flaps * 533)])
                            event_to_trigger = ae.find("FLAPS_SET")  # Sets AP autopilot hold level
                            event_to_trigger(last_flaps * 533)
                            #fsuipc.write([(65759, "l",0)])
                if val[0:2] == b'A0':
                    value = int(val[3:])
                    print ("A0 %i" % (value))
                  #  event_a0(value)
                if val[0:2] == b'A1':
                    value = int(val[3:])
                    print ("A1 %i" % (value))
                   # event_a1(value)
                if val[0:2] == b'A2':
                    value = int(val[3:])
                    print ("A2 %i" % (value))
                if val[0:2] == b'A3':
                    value = int(val[3:])
                    print ("A3 %i" % (value))