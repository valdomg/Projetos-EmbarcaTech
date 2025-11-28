import requests
import logging
from dotenv import load_dotenv
import os
from MongoDB.MongoDBConnection import MongoDBConnection
from datetime import datetime

load_dotenv()

mongo__conn = MongoDBConnection(os.getenv('MONGO_URI'), os.getenv('MONGO_DATABASE'))

def check_if_device_exists(device:str) -> bool:
    '''
    Verifica se existe um device na database
    '''

    if mongo__conn.start_connection() == False:
        logging.warning('Banco de dados não conectado')
        mongo__conn.close_connection()
        return

    result = mongo__conn.check_if_document_exists('devices', 'device', device)

    if result:
        logging.info(f'Device [{device}] no banco de dados')
        mongo__conn.close_connection()
        return True
    
    logging.info(f'Device [{device}] não registrado no banco de dados')
    mongo__conn.close_connection()
    return False

def register_chamada_mongodb(payload:dict):
    '''
    Registra uma nova chamada na database, realiza o tratamento do payload
    '''

    dispositivo_id = payload.get('id')
    mensagem = payload.get('mensagem')
    room_number = payload.get('room_number')
    local_emergencia = payload.get('local')

    document_to_save = {
        'dispositivo_id': dispositivo_id,
        'local': local_emergencia,
        'sala': room_number,
        'data': datetime.now()
    }

    logging.info(f'Mensagem do dispositivo {dispositivo_id}:{mensagem}')

    try:
        if mongo__conn.start_connection() == False:
            logging.warning('Banco de dados não conectado')
            mongo__conn.close_connection()
            return 

        result = mongo__conn.insert_document_collection('chamadas', document_to_save)

        if result:
            logging.info('Chamada registrada no banco de dados')

    except Exception as e:
        logging.exception(e)

    finally:
        mongo__conn.close_connection()

def register_status_device_mongodb(device:str, payload:dict):
    '''
    Registra o status de um device na database
    '''

    if not 'status' in payload:
        return
     
    document_to_save = {
        'device': device,
        'status': payload.get('status'),
        'updateAt': datetime.now()
    }

    mongo__conn.start_connection()
    
    result = mongo__conn.return_document('status_device', 'device', device)

    if not result:    
        logging.warning(f'Status de device [{device}] não encontrado na database!')

        logging.info(f'Registrando status do [{device}]')    
        mongo__conn.insert_document_collection('status_device', document_to_save)

        return

    logging.info(f'documento de status do device [{device}] já existe, atualizando')

    id_status_device = str(result['_id'])
    mongo__conn.update_document_by_id('status_device', id_status_device, document_to_save)
    
    mongo__conn.close_connection()

    return True

def register_status_chamada_mongo_db(device:str, payload:dict):
    '''
    Função para registrar/atualizar uma chamada na tabela de status_chamadas
    '''

    if 'status' in payload:
        return

    room_number = payload.get('room_number')
    status = payload.get('estado')
    
    document = {
        'device': device,
        'room_number': room_number,
        'status': status,
        'updateAt': datetime.now()
    }
    
    if mongo__conn.start_connection() == False:
        logging.warning('Banco de dados não conectado')
        mongo__conn.close_connection()
        return
    try:
        result = mongo__conn.return_document('status_chamadas', 'device', device)

        print(result)

        if not result:        
            logging.warning(f'Device [{device}] não encontrado no status de chamada!')

            logging.info(f'Registrando status de chamada no mapa de [{device}]')    
            mongo__conn.insert_document_collection('status_chamadas', document)

            return 

        logging.info(f'documento de status de [{device}] já existe, atualizando')

        id_chamada = str(result['_id'])
        mongo__conn.update_document_by_id('status_chamadas', id_chamada, document)
    except Exception as e:
        print(e)

    finally:
        mongo__conn.close_connection()


