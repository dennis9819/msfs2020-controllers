class FSSCPHandler:
    name = ""

    def __init__(self,controller):
        self.controller = controller
    
    def handle(self):
        print("Started handler for '%s' @ %s" % (self.name,self.controller.name))
        pass

    def getName(self):
        return self.name