import serial
import _thread

class FSSCPDevice:
    CMD_IDENT = 0x01
    CMD_RESET = 0x3F
    CMD_DATA_WR = 0x02
    CMD_DATA_RD = 0x82
    CMD_ID_WR = 0x84

    def sendCommand(self, cmd, data = []):
        tries = 3
        while tries > 0:
            packet = bytearray([cmd])
            packet.append(len(data))
            for b in data:
                packet.append(b)
            print(packet)
            self.serial.write(packet)
            # read respone
            rlen = int.from_bytes(self.serial.read())
            if (rlen < 255):
                return self.serial.read(rlen)


    def listenEvent(self):
        _relenRaw = s.read()
        if (len(_relenRaw) > 0):
            rlen = int.from_bytes(s.read())
            return self.serial.read(rlen)
        else:
            return bytearray()

    def __init__(self,name: str,port: str,controller):
        self.serial = serial.Serial()
        self.serial.port = port
        self.serial.baudrate = 19200
        self.serial.parity = serial.PARITY_NONE
        self.serial.bytesize = serial.EIGHTBITS
        self.serial.stopbits = serial.STOPBITS_ONE
        self.serial.timeout = 1.5
        self.serial.xonxoff = False
        self.serial.rtscts = False
        self.serial.dsrdtr = False
        self.controller = controller
        self.serial.open()
        rlen = int.from_bytes(self.serial.read())
        data = self.serial.read(rlen)

        self.name = name
        self.id = int.from_bytes(data[0:2],  byteorder='little')
        self.type = int.from_bytes(data[2:4],  byteorder='little')
        print(" -> DETECTED: %s / ID  0x%s (%i) / TYPE 0x%s (%i)" % (name,data[0:2].hex(),self.id,data[2:4].hex(),self.type))

        fn = controller.getControllerMapping(self.type)
        if fn == None:
            self.die("No controller mapping for %i" % (self.type))
        else:
            _thread.start_new_thread( fn, (self) )

    def die(self, reason = "die()"):
        print(" -> CONTROLLER REMOVED: %s / CAUSE: %s" % (self.name,reason))
        self.serial.close()
        del self

class FSSCPDeviceController:
    def __init__(self):
        self.devices = []
        self.mappings = {}
        _thread.start_new_thread( self.scanNew, () )

    def addControllerMapping(self, id: int, fn):
        self.mappings[id] = fn

    def getControllerMapping(self, id: int):
        if id in list(self.mappings.keys()):
            return self.mappings[id]
        else:
            return None

    def scanNew(self):
        while True:
            ports = ['COM%s' % (i + 1) for i in range(256)]
            for port in ports:
                try:
                    newDevice= FSSCPDevice(port, port, self)
                    self.devices.append(newDevice)
                except (OSError, serial.SerialException):
                    pass

test = FSSCPDeviceController()
input("Press key\n")
    