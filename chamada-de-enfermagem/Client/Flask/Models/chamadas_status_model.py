import logging
from MongoDB.MongoDBConnection import MongoDBConnection

class ChamadasStatusModel:
    '''
    Classe para retornar documentos da collection status_chamadas
    '''
    def __init__(self, db:MongoDBConnection):
        self.db = db

    def return_docs_from_status_chamadas(self):
        '''
        Retorna todos os documentos da collection
        '''
        return self.db.list_all_documents_from_collection('status_chamadas')


    def return_docs_status(self, status:str):
        '''
        Retornar os documentos por status do documento da collection
        '''
        docs = self.db.list_all_documents_from_collection('status_chamadas')
        docs_status = {}

        if not docs:
            return False

        for index in docs:
            if index['status'] == status:
                docs_status = index

        return docs_status
