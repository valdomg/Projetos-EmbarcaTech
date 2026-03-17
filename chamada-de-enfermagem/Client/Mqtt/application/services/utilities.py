import requests
import logging
from dotenv import load_dotenv
import os
from MongoDB.mongo_conn import mongo_conn 
from datetime import datetime
import paho.mqtt.client as mqtt
import json

load_dotenv()

broker = os.getenv('BROKER_IP')
port = int(os.getenv('BROKER_PORT'))
user_name = os.getenv('PUB_USERNAME')
password = os.getenv('PUB_PASS')

def check_if_device_exists(device:str) -> bool:
    '''
    Verifica se existe um device na database
    '''
    try:

        result = mongo_conn.check_if_document_exists('devices', 'device', device)

        if result:
            logging.info(f'Device [{device}] no banco de dados')
            return True

        logging.info(f'Device [{device}] não registrado no banco de dados')

    except Exception as e:
        logging.exception(e)

    return False

def register_chamada_mongodb(payload:dict):
    '''
    Registra uma nova chamada na database, realiza o tratamento do payload
    '''

    dispositivo_id = payload.get('id')
    mensagem = payload.get('mensagem')
    room_number = int(payload.get('room_number'))
    local_emergencia = payload.get('local')
    status = payload.get('estado')
    
    if status == 'ocioso':
        return 

    document_to_save = {
        'dispositivo_id': dispositivo_id,
        'local': local_emergencia,
        'sala': room_number,
        'data': datetime.now()
    }

    logging.info(f'Mensagem do dispositivo {dispositivo_id}:{mensagem}')

    try:
        result = mongo_conn.insert_document_collection('chamadas', document_to_save)

        if result:
            logging.info('Chamada registrada no banco de dados')

    except Exception as e:
        logging.exception(e)

def register_status_device_mongodb(device:str, payload:dict):
    '''
    Registra o status de um device na database
    '''

    if 'room_number' in payload:
        return
     
    document_to_save = {
        'device': device,
        'status': payload.get('status'),
        'updateAt': datetime.now()
    }

    '''
    Removendo implementação de publicação de mensagem vazia para apagar mensagens retidas
    '''
    try:

        result = mongo_conn.return_document('status_device', 'device', device)

        if not result:    
            logging.warning(f'Status de device [{device}] não encontrado na database!')

            logging.info(f'Registrando status do [{device}]')    
            mongo_conn.insert_document_collection('status_device', document_to_save)

            return

        logging.info(f'documento de status do device [{device}] já existe, atualizando')

        id_status_device = str(result['_id'])
        mongo_conn.update_document_by_id('status_device', id_status_device, document_to_save)

        #publish_message_on_topic_avoid_retain_messages(topic_to_publish)
        #publish_message_on_topic_avoid_retain_messages(topic_to_publish_on_enfermaria)

    except Exception as e:
        logging.exception(e)

    return True

def register_status_chamada_mongo_db(device:str, payload:dict):
    '''
    Função para registrar/atualizar uma chamada na tabela de status_chamadas
    '''

    if not ('room_number' in payload) and ('estado' not in payload):
        return 
    
    logging.info(payload)

    room_number = int(payload.get('room_number'))
    estado = payload.get('estado')
    local = payload.get('local')
    
    document = {
        'device': device,
        'room_number': room_number,
        'status': estado,
        'local': local,
        'updateAt': datetime.now()
    }
    
    try:
        result = mongo_conn.return_document('status_chamadas', 'device', device)

        print(f':RESULTADO:{result}')

        if not result:        
            logging.warning(f'Device [{device}] não encontrado no status de chamada!')

            logging.info(f'Registrando status de chamada no mapa de [{device}]')    
            mongo_conn.insert_document_collection('status_chamadas', document)

            return 

        logging.info(f'documento de status de [{device}] já existe, atualizando')

        id_chamada = str(result['_id'])
        mongo_conn.update_document_by_id('status_chamadas', id_chamada, document)

    except Exception as e:
        print(e)

def publish_message_to_stop_emergency(data:dict):

    device = data['device']
    room_number = data['room_number']
    local = data['local']

    result = mongo_conn.return_document('status_chamadas', 'device', device)

    print('Enviando mensagem para parar emergência')

    if not result:
        return False
    
    if result['status'] == 'ocioso':
        return False
    
    topic = f'dispositivos/atendimento/{device}'

    temp_payload = {
        'atendimento': 'atendido'
    }

    result = public_topic_message(topic, temp_payload, False)
    
    return result


def publish_message_confimation(device:str, payload:dict):
    topic = f'dispositivos/confirmacao/{device}'

    status = payload.get('estado')

    if status != 'emergencia':
        return
    
    payload_to_send = {'status': 'ok'}
    result = public_topic_message(topic, payload_to_send, retain_message=True)

    logging.info(f'{result}')

    return result

def publish_message_to_not_retain_confirmation(device:str, payload:dict):
    topic = f'dispositivos/confirmacao/{device}'

    status = payload.get('estado')

    if status != 'ocioso':
        return
    
    payload = ''    
    result_confirm, result_atendimento = public_topic_message(topic, payload, True), publish_message_to_end_atendimento(device)

    return result_confirm, result_atendimento

def publish_message_to_end_atendimento(device:str):
    topic = f'dispositivos/atendimento/{device}'

    payload = {'atendimento': 'finalizado'}

    result = public_topic_message(topic, payload, False)

    return result

def public_topic_message(topic:str, payload:dict, retain_message:bool):

    if payload == '':
        payload_temp = ''

    else:
        payload_temp = json.dumps(payload)

    client = mqtt.Client(client_id=user_name)
    client.username_pw_set(user_name, password)
    client.connect(broker, port)

    result = client.publish(topic, payload=payload_temp, retain=retain_message)
    logging.info(f'Mensagem publicada em {topic}, payload: {payload}')

    client.disconnect()
    return result
'''
def publish_message_on_topic_avoid_retain_messages(topic: str):
    """Publica uma mensagem MQTT no tópico especificado."""
    client = mqtt.Client(client_id=user_name)
    client.username_pw_set(user_name, password)
    client.connect(broker, port)

    client.publish(topic, payload=None, retain=True)
    client.disconnect()
    logging.info(f'Mensagem publicada em {topic}')
'''