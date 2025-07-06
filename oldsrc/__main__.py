from mod_com1 import mod_com1_load
from mod_trim import mod_load
from mod_alt import mod_alt_load
import serial
import time
import _thread

class ControllerDevice:
    def __init__(self,name,serial):
        self.serial = serial
        self.name = name
        self.id = ""
        self.type = ""

    def sendCommand(self,command):
        for i in range(0,3):
            self.serial.write(command)
            time.sleep(0.01)
            responses = []
            while self.serial.in_waiting:
                val = str(self.serial.readline())
                if val.startswith('b\'+'):
                    responses.append(val[3:-5])
            if len(responses) > 0:
                return responses
            else:
                print("Device Timeout @ %s (Retries %i)" % (self.name,i))
        return responses
            
    def getIdent(self):
        data = self.sendCommand(b'GS IDENT\n')
        self.id = int(data[0], 0)
        ident_str = data[0]
        data = self.sendCommand(b'GS TYPE\n')
        self.type = int(data[0], 0)
        print(" -> ID %s (%i) / TYPE %s (%i)" % (ident_str,self.id,data[0],self.type))

        if self.type == 225:
            print(" -> Detected TRIM_SECTION. Starting worker thread...")
            self.thread = _thread.start_new_thread( mod_load, (self,) )
        if self.type == 100:
            print(" -> Detected COM. Starting worker thread...")
            self.thread = _thread.start_new_thread( mod_com1_load, (self,) )
        if self.type == 0:
            print(" -> Detected ALT. Starting worker thread...")
            self.thread = _thread.start_new_thread( mod_alt_load, (self,) )

    def testConnected(self):
        try:
            self.sendCommand(b'GS IDENT\n')
        except:
            print("Device Disconnected @ %s" % (self.name))
            return False
        return True



## Hpusekeeping
controllerDevices = []
     



def scanNew():
    while True:
        time.sleep(2)
        ports = ['COM%s' % (i + 1) for i in range(256)]
        for port in ports:
            try:
                s = serial.Serial(port,115200, timeout=1)
                addSerialPort(port,s)
            except (OSError, serial.SerialException):
                pass

def scanHeartbeat():
    while True:
        time.sleep(10)
        #for i in range(0,len(controllerDevices)):
        #    if not controllerDevices[i].testConnected():
        #        del controllerDevices[i]

def addSerialPort(name,serial):
    found = 0
    for device in controllerDevices:
        if device.name == name:
            found += 1
    if found == 0:
        # get device id and type
        time.sleep(2)
        print("Found new Device @ %s" % (name))
        newDevice = ControllerDevice(name,serial)
        newDevice.getIdent()
        controllerDevices.append(newDevice)
    else:
        serial.close()

_thread.start_new_thread( scanNew, () )
_thread.start_new_thread( scanHeartbeat, () )

input("Press key\n")




