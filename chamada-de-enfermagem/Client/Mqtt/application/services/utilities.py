import requests
import logging
from dotenv import load_dotenv
import os
from MongoDB.MongoDBConnection import MongoDBConnection
from datetime import datetime

load_dotenv()

mongo__conn = MongoDBConnection(os.getenv('MONGO_URI'), os.getenv('MONGO_DATABASE'))

def register_call_mongo_db(device:str, room_number:int, status:str):
    '''
    Função para registrar/atualizar uma chamada na tabela de status_chamadas
    '''
    document = {
        'device': device,
        'room_number': room_number,
        'status': status,
        'updateAt': datetime.now()
    }

    mongo__conn.start_connection()
    if mongo__conn.check_if_document_exists('status_chamadas', 'device', device):
        
        logging.info('documento já existe, atualizando')

        result = mongo__conn.return_document('status_chamadas', 'device', device)

        if not result:    
            logging.WARNING('Device não encontrado!')

        id_chamada = str(result['_id'])

        mongo__conn.update_document_by_id('status_chamadas', id_chamada, document)

    else:
        logging.info(f'Registrando status no mapa de {device}')    
        mongo__conn.insert_document_collection('status_chamadas', document)

    mongo__conn.close_connection()
