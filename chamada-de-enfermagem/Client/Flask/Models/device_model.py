from datetime import datetime

'''
Classe para instanciar device
'''

class Device():
    def __init__(self, device:str, room_number:int):
        self.device = device
        self.room_number = room_number
        self.createdAt = datetime.now()
    
    def getDevice(self):
        return self.device
    
    def getRoomNumber(self):
        return self.room_number
    
    def getCreatedAt(self):
        return self.createdAt