
from modules.FSSCPHandler import FSSCPHandler

class C172_TRIM(FSSCPHandler):
    
    name = "C172 Trim Section"
    def handler(device):
        while True:
            data = device.listenEvent()
            print(data)