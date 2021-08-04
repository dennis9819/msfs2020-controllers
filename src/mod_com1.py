from fsuipc import FSUIPC
import serial
import time

com1_active = list(b'0000')
com1_stby = list(b'0000')
com2_active = list(b'0000')
com2_stby = list(b'0000')

def getbcd(val):
    val = ((val[0] & 0x0F) << 12) | ((val[1] & 0x0F) << 8) | ((val[2] & 0x0F) << 4) | (val[3] & 0x0F) 
    print(val)
    return val

def mod_com1_load(controllerDevice):
   with FSUIPC() as fsuipc:
        time.sleep(4)
        while True:

            while controllerDevice.serial.in_waiting:  # Or: while ser.inWaiting():
                val = controllerDevice.serial.readline().decode("utf-8") 
            # print(val)
                if val.startswith("EV F2AL"):
                    num = int(int(val[8:-2])/10)
                    com2_active[2] = int(num/10)
                    com2_active[3] = num%10
                    fsuipc.write([(0x3118, "H", getbcd(com2_active))])
                    
                if val.startswith("EV F2AH"):
                    num = int(int(val[8:-2])-100)
                    com2_active[0] = int(num/10)
                    com2_active[1] = num%10
                    fsuipc.write([(0x3118, "H", getbcd(com2_active))])

                if val.startswith("EV F2SL"):
                    num = int(int(val[8:-2])/10)
                    com2_stby[2] = int(num/10)
                    com2_stby[3] = num%10
                    fsuipc.write([(0x311C, "H", getbcd(com2_stby))])
                    
                if val.startswith("EV F2SH"):
                    num = int(int(val[8:-2])-100)
                    com2_stby[0] = int(num/10)
                    com2_stby[1] = num%10
                    fsuipc.write([(0x311C, "H", getbcd(com2_stby))])

                if val.startswith("EV F1AL"):
                    num = int(int(val[8:-2])/10)
                    com1_active[2] = int(num/10)
                    com1_active[3] = num%10
                    fsuipc.write([(0x034E, "H", getbcd(com1_active))])
                    
                if val.startswith("EV F1AH"):
                    num = int(int(val[8:-2])-100)
                    com1_active[0] = int(num/10)
                    com1_active[1] = num%10
                    fsuipc.write([(0x034E, "H", getbcd(com1_active))])

                if val.startswith("EV F1SL"):
                    num = int(int(val[8:-2])/10)
                    com1_stby[2] = int(num/10)
                    com1_stby[3] = num%10
                    fsuipc.write([(0x311A, "H", getbcd(com1_stby))])
                    
                if val.startswith("EV F1SH"):
                    num = int(int(val[8:-2])-100)
                    com1_stby[0] = int(num/10)
                    com1_stby[1] = num%10
                    fsuipc.write([(0x311A, "H", getbcd(com1_stby))])


