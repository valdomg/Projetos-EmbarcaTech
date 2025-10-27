import json
import os
from dotenv import load_dotenv
from MongoDB.MongoDBConnection import MongoDBConnection
from Mqtt.application.services.utilities import send_update_to_flask 
from datetime import datetime
import logging
from time import sleep

load_dotenv()
topic = os.getenv('BROKER_TOPIC')
uri = os.getenv('MONGO_URI') 
database = os.getenv('MONGO_DATABASE')

mongo = MongoDBConnection(uri, database)

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

def on_message(client, userdata, message, properties=None):
    '''
    Callback para formatar as mensagens recebidas nos tópicos

        Os dispositivos nas salas assinam apenas o tópico em que estão,
        dessa forma, dentro do payload podem receber comandos de desligar/ligar ou qualquer outro
        ex: dispositivos/enfermaria/seu_id

        E publicam apenas no tópico de posto_enfermaria
        dessa podem enviar seu estado ligado/timeout
        ex:dispositivos/posto_enfermaria/seu_id

        o formato de payload será assim: {
            'id_dispositivo':'id_dispositivo',
            'estado':'emergência/timeout',
            'mensagem':'mensagem para debug',
            'local': 'sala do dispositivo',
            'comando': 'ligar/desligar'
        }
    '''
        
    try:
        payload = json.loads(message.payload.decode())
    except json.JSONDecodeError:
        logging.error(f'Falha ao decodificar mensagem..')
        return
    logging.info(f'Mensagem recebida!')

    # Quebra o tópico em partes
    partes = message.topic.split('/')
    _, local_topic, dispositivo_id = partes

    if mongo.start_connection() == False:
        mongo.close_connection()
        return 
        
    if mongo.check_if_document_exists('devices','device', dispositivo_id) == False:
        logging.info('Dispositivo não encontrado na base de dados...')
        mongo.close_connection() 
        return 
        
    logging.info('Dispositivo logado!')
        
    comando = payload.get('comando')
    local_emergencia = payload.get('local')
    room_number = payload.get('room_number')
    estado = payload.get('estado')

    if local_topic == 'posto_enfermaria' and comando == 'ligar':

        '''
        Laço condicional para registrar chamada no banco de dados
        '''

        document= {
            'dispositivo_id': dispositivo_id,
            'local': local_emergencia,
            'enfermaria': room_number,
            'data': datetime.now()
        }

        result = mongo.insert_document_collection('chamadas', document)
        
        if result:
            logging.warning('Chamada inserida no banco de dados!')
        
    if local_topic == 'enfermaria':
        logging.info(f'Mensagem do tópico: {local_topic}')
    
    send_update_to_flask(room_number, estado)
    mongo.close_connection()

def on_subscribe(client, userdata, mid, granted_os, properties=None):
    '''
    Callback para conferir se a inscrição em algum tópico foi bem sucedida
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

