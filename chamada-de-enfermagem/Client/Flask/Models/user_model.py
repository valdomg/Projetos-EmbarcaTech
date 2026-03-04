from datetime import datetime

'''
Classe para instanciar usuário
'''

class User():
    def __init__(self, username:str, password:str, role:str):
        
        self.username = username
        self.password = password
        self.role = role
        self.createdAt = datetime.now()
    
    def getUsername(self):
        return self.username
    
    def getPassword(self):
        return self.password
    
    def getCreateAt(self):
        return self.createdAt

    def getRole(self):
        return self.role