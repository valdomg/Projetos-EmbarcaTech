 
from pymongo.mongo_client import MongoClient, PyMongoError, ConnectionFailure
from pymongo.server_api import ServerApi
from bson.objectid import ObjectId
import os
from datetime import datetime


class MongoDBConnection:
    '''
    Classe para conexão com banco de dados MongoDB
    '''
    def __init__(self, uri:str, database_name:str):
        self.uri = uri
        self.database_name = database_name
        self.client = None
        self.db = None

    def start_connection(self):
        '''
        Função de iniciar a conexão com o banco de dados
        '''
        
        if self.uri != None and self.database_name != None:
            try:
                self.client = MongoClient(self.uri, server_api=ServerApi('1'))
                self.db = self.client[self.database_name]
                self.client.admin.command('ping')
                print("Pinged your deployment. You successfully connected to MongoDB!")
                return True
            except ConnectionFailure as e:
                self.client = None
                self.db = None
                print(e)
                return False
        else:
            print('Database não definida')
            return False
    
    def list_all_documents_from_collection(self, collection:str):
        '''
        Lista os documentos na coleção
        '''
        try:
            if self.client is not None:
                collection = self.db[collection]
                return list(collection.find())
            else:
                print('Client not connected')
        except PyMongoError as e:
            print('Error in list documents...')
            print(e)

    def count_all_documents_on_collection(self, collection:str):
        '''
        Retorna um valor com a quantidade de documentos na Database
        '''
        
        try:
            if self.client is not None:
                collection = self.db[collection]

                return collection.count_documents({})
            
            else:
                print('Client is not connected')

        except PyMongoError as e:
            print('Error in count documents')
            print(e)

    def count_documents_by_date(self, collection:str, label_data:str, start_date:datetime, end_date:datetime):
        '''
        Retorna um valor com a quantidade de documentos em algum período
        '''
        try:
            if self.client is not None:
                collection = self.db[collection]
                return collection.count_documents({
                        label_data:{
                            '$gte': start_date,
                            '$lte': end_date
                        }
                    })
            else:
                print('Client not connected')
        except PyMongoError as e:
            print('Error in list documents...')
            print(e)        
   
    def check_if_document_exists(self, collection:str, label:str, value:str):
        '''
        Verifica a existência de algum dado no banco de dados
        '''

        try:
            if self.client is not None:
                collection_to_search = self.db[collection]

                result = collection_to_search.find_one({label:value})

                if result is not None:
                    return True  
                
                return False              
        except PyMongoError as e:
            print('Error in check values...')
            print(e)
            
        return False
    
    def check_if_document_exists_by_id(self, collection:str, document_id:str):
        '''
        Verifica a existência de algum dado no banco de dados
        '''

        try:
            if self.client is not None:
                collection_to_search = self.db[collection]

                result = collection_to_search.find_one({'_id':ObjectId(document_id)})

                if result is not None:
                    return True                
        except PyMongoError as e:
            print('Error in check values...')
            print(e)
            
        return False
    
    def return_document(self,collection:str, label_to_search:str, value_to_match:str) -> dict:
        '''
        Retorna um documento de acordo com seu label
        '''
        try:
            if self.client is not None:
                collection_to_search = self.db[collection]

                result = collection_to_search.find_one({label_to_search:value_to_match})

                if result == None:
                    return False

                return result

        except PyMongoError as e:
            print('Error in check values...')
            print(e)
            
        return False
    
    def return_document_by_id(self, collection:str, id:str):
        '''
        Retorna um documento de acordo com seu ID
        '''
        try:
            if self.client is not None:

                collection_to_search = self.db[collection]

                result = collection_to_search.find_one({'_id':ObjectId(id)})

                return result
            
        except PyMongoError as e:
            print('Error in check values...')
            print(e)

    def list_documents_by_date(self, collection:str, label_data:str,start_date:datetime, end_date:datetime):
        '''
        Retorna uma lista com documentos com uma query de data
        '''
        try:
            if self.client is not None:
                collection = self.db[collection]
                return list (collection.find({
                        label_data:{
                            '$gte': start_date,
                            '$lte': end_date
                        }
                    })
                )
            else:
                print('Client not connected')
        except PyMongoError as e:
            print('Error in list documents...')
            print(e)

    def insert_document_collection(self,collection:str, document: dict):
        '''
        Insere um novo documento na coleção
        '''
        try:
            if self.client is not None:
                document_to_save = document 

                collection = self.db[collection]
                result = collection.insert_one(document_to_save)

                return result

            else:
                print('Client not connected')

        except PyMongoError as e:
            print('Error on insert document...')
            print(e) 

    def update_document_by_id(self,collection:str, document_id:str, document_with_updates:dict) -> bool:
        '''
        Funçao para atualizar um documento de acordo com seu ID
        '''
        try:
            if self.client is None:
                print('Client not connected')

            document_to_update = self.return_document_by_id(collection, document_id)

            if document_to_update == None:
                print('Not value in DB')
                return False

            if self.check_if_docs_is_equal(document_with_updates, document_to_update) == True:
                print('Docs is the same, no update')
                return False
                
            collection_update = self.db[collection]

            result = collection_update.update_one(
                {'_id': ObjectId(document_id)},
                {'$set': document_with_updates}
            )

            if result == None:
                return False
            
            return True
            
        except PyMongoError as e:
            print('Error in update values')
            print(e)
        
        pass

    def delete_document(self, collection:str, label_to_match:str, value_to_match:str) -> bool:
        '''
        Função para deletar um documento da database de acordo com um label e seu valor
        '''
        try:
            if self.client is not None:
                
                if self.check_if_document_exists(collection, label_to_match, value_to_match) == False:
                    print('No values in DB')
                    return False
                
                collection_delete = self.db[collection]
                result = collection_delete.delete_one({label_to_match: value_to_match})

                if result == False:
                    print(f'Error in delete value: {result}')
                    return False
                
                print(f'Successfully in delete value: {result}')
                return True
            
            else:
                print('Client not connected')

        except PyMongoError as e:
            print('Error in delete value')
            print(e)
            return False
        
    def delete_document_by_id(self, collection:str, document_id:str) -> bool:
        '''
        Função para deletar um documento da database de acordo com seu id
        '''
        try:
            if self.client is not None:
                
                if self.check_if_document_exists_by_id(collection, document_id) == False:
                    print('No values in DB')
                    return False
                
                collection_delete = self.db[collection]
                result = collection_delete.delete_one({'_id': ObjectId(document_id)})

                if result == False:
                    print(f'Error in delete value: {result}')
                    return False
                
                print(f'Successfully in delete value: {result}')
                return True
            
            else:
                print('Client not connected')

        except PyMongoError as e:
            print('Error in delete value')
            print(e)
            return False
        
    def check_if_docs_is_equal(self, document_one:dict, document_two:dict) -> bool:
        '''
        Checa se dois documentos do mongodb são iguais
        '''
        same_values = all(document_one[key] == document_two[key] for key in document_one if key in document_two)

    def close_connection(self):
        '''
        Função para fechar a conexão com o banco de dados
        '''
        if self.client:
            self.client.close()
            print("Conexão fechada.")

