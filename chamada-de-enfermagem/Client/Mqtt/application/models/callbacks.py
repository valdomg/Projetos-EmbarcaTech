import json
import os
from dotenv import load_dotenv
from Mqtt.application.services.utilities import register_chamada_mongodb, register_status_chamada_mongo_db, check_if_device_exists, register_status_device_mongodb
from datetime import datetime
import logging
from time import sleep

load_dotenv()
topic = os.getenv('BROKER_TOPIC')

def on_connect(client, userdata, flags, rc, properties=None):
    '''
    Callback usado ao conectar o client a algum tópico do broker
    '''
    if rc != 0:
        logging.error('Conexão mal sucedida, erro:', rc, '\n Tentando novamente...')
        reconnect_mqtt(client)

    client.subscribe(topic)

def on_disconnect(client, userdata, flags, rc, properties=None):
    '''
    Callback para quando houver desconexões inesperadas
    '''
    if rc != 0:
        logging.error('Desconexão inesperada, tentando novamente...', rc)
        
    logging.info('Tentando novamente...')
    
    try:
        reconnect_mqtt(client)

    except Exception as e:
        logging.exception('Conexão mal sucedida, erro:', rc)

'''
Colocar nova função para registar se 
'''
def on_message(client, userdata, message, properties=None):
    '''
    Callback para formatar as mensagens recebidas nos tópicos

        Os dispositivos nas salas assinam apenas o tópico em que estão,
        dessa forma, dentro do payload podem receber comandos de desligar/ligar ou qualquer outro
        ex: dispositivos/enfermaria/seu_id

        E publicam apenas no tópico de posto_enfermaria
        dessa podem enviar seu estado ligado/timeout
        ex:dispositivos/posto_enfermaria/seu_id

        o formato de payload será assim para os microcontroladores: {
            'id':'id_dispositivo',
            'estado':'emergência/oscioso',
            'mensagem':'mensagem para debug',
            'room_number': 'número da sala'
            'local': 'Bloco/Ala/Região',
            'comando': 'ligar/desligar'
        }

        formato para status: {
            'status': 'ok/error'
        }
    '''
        
    try:
        payload = json.loads(message.payload.decode())
    except json.JSONDecodeError:
        logging.error(f'Falha ao decodificar mensagem..')
        return
    
    logging.info(f'Mensagem recebida!')

    '''Quebra o tópico em partes'''
    partes = message.topic.split('/')
    logging.info(f'Partes:{partes}')
    
    dispositivo_topic = None

    if len(partes) == 2:
        _, local_topic = partes

    else:
        _,local_topic, dispositivo_topic = partes

    device = payload.get('id')
    logging.info(f'Payload: {payload}')

    if dispositivo_topic != None:
        device = dispositivo_topic

    if check_if_device_exists(device) == False:
        logging.warning(f'Device com id {device} não encontrado!')
        return

    if local_topic == 'confirmacao':
        register_status_device_mongodb(dispositivo_topic, payload)

    if local_topic == 'posto_enfermaria':
        '''
        Laço condicional para registrar chamada no banco de dados
        '''
        register_chamada_mongodb(payload)
        
    if local_topic == 'enfermaria':
        logging.info(f'Mensagem para o tópico: {local_topic}')

    
    register_status_chamada_mongo_db(device, payload)

    return None

    

def on_subscribe(client, userdata, mid, granted_os, properties=None):
    '''
    Callback para conferir s'e a inscrição em algum tópico foi bem sucedida
    '''
    logging.info(f'Inscrito com sucesso no tópico: {topic}')


def reconnect_mqtt(client):
    '''
    Função de reconexão do client com mqtt
    '''
    delay = 1
    while True:
        try:
            client.reconnect()
            logging.info('[MQTT] Reconectado com sucesso!')
            return
        except Exception as e:
            logging.exception('[MQTT] Falha ao reconectar:', e)
            sleep(delay)
            delay = min(delay * 2, 60)  # Máximo 60 segundos

