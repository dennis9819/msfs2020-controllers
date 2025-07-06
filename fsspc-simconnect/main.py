
from modules.C172_TRIM import C172_TRIM
from libs.pyfsscp2 import FSSCPDeviceController 

devices = FSSCPDeviceController()
devices.addControllerMapping(13330,C172_TRIM)
input("Press key\n")
    