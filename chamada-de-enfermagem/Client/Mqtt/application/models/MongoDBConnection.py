 
from pymongo.mongo_client import MongoClient, PyMongoError, ConnectionFailure
from pymongo.server_api import ServerApi
from dotenv import load_dotenv
import os
from datetime import datetime

load_dotenv()
'''
Classe para conexão com banco de dados MongoDB
'''

class MongoDBConnection:

    def __init__(self, uri:str, database_name:str, collection:str):
        self.uri = uri
        self.database_name = database_name
        self.collection = collection
        self.client = None
        self.db = None

    '''
    Função de iniciar a conexão com o banco de dados
    '''
    def start_connection(self):
        
        try:
            self.client = MongoClient(self.uri, server_api=ServerApi('1'))
            self.db = self.client[self.database_name]
            self.client.admin.command('ping')
            print("Pinged your deployment. You successfully connected to MongoDB!")
        except ConnectionFailure as e:
            self.client = None
            self.db = None
            print('Error on conennection...')
            print(e)
        
    
    '''
    Insere um novo documento na coleção
    '''
    def insert_document_collection(self,collection:str, local:str, room_number:int):

        try:
            if self.client is not None:
                document = {
                    local: room_number,
                    'data': datetime.now() 
                }

                collection = self.db[self.collection]
                result = collection.insert_one(document)

                print(result)

            else:
                print('Client not connected')

        except PyMongoError as e:
            print('Error on insert document...')
            print(e)
    '''
    Lista os documentos na coleção
    '''
    def list_documents(self):
        try:
            if self.client is not None:
                collection = self.db[self.collection]
                return list(collection.find())
            else:
                print('Client not connected')
        except PyMongoError as e:
            print('Error in list documents...')
            print(e)
    '''
    Verifica a existência de algum dado no banco de dados
    '''
    def check_if_document_exists(self, collection:str, device:str):

        try:
            if self.client is not None:
                collection = self.db[collection]

                result = collection.find_one({"device": device})

                if result is not None:
                    return True
        except PyMongoError as e:
            print('Error in check values...')
            print(e)    
        return False
    
    '''
    Função para fechar a conexão com o banco de dados
    '''
    def close_connection(self):
        if self.client:
            self.client.close()
            print("Conexão fechada.")
