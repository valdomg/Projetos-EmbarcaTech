import json
import os
from dotenv import load_dotenv
from Mqtt.application.models.MongoDBConnection import MongoDBConnection
from datetime import datetime
load_dotenv()

topic = os.getenv('BROKER_TOPIC')
uri = os.getenv('MONGO_URI') 
database = os.getenv('MONGO_DATABASE')

mongo = MongoDBConnection(uri, database)
'''
Callback usado ao conectar o client a algum tópico do broker
'''
def on_connect(client, userdata, flags, rc, properties=None):
    if rc != 0:
        print('Conexão mal sucedida, erro:', rc, '\n Tentando novamente...')

    client.subscribe(topic)

def on_disconnect(client, userdata, flags, rc, properties=None):
    if rc != 0:
        print('Desconexão inesperada, tentando novamente...', rc)
        
    print('Tentando novamente...')
    try:
        client.reconnect()

    except Exception as e:
        print('Conexão mal sucedida, erro:', rc)


'''
    Callback para formatar as mensagens recebidas nos tópicos

    Os dispositivos nas salas assim apenas o tópico em que estão,
    dessa forma, dentro do payload podem receber comandos de desligar/ligar ou qualquer outro
    ex: dispositivos/enfermagem/seu_id

    E publicam apenas no tópico de posto_enfermagem
    dessa foram podem enviar seu estado ligado/timeout
    ex:dispositivos/posto_enfermagem/seu_id

    o formato de payload será assim: {
        'id_dispositivo':'id_dispositivo',
        'estado':'emergência/timeout',
        'mensagem':'mensagem para debug',
        'source': 'sala do dispositivo',
        'comando': 'ligar/desligar'
    }
'''

def on_message(client, userdata, message, properties=None):
    payload = json.loads(message.payload.decode())
    print(f'Mensagem recebida!')

    # Quebra o tópico em partes
    partes = message.topic.split('/')
    _, local_topic, dispositivo_id = partes

    if mongo.start_connection() == False:
        mongo.close_connection()
        return 
        
    if mongo.check_if_document_exists('devices','device', dispositivo_id) != True:
        print('Dispositivo não encontrado na base de dados...')
        mongo.close_connection() 
        return 
        
    print('Dispositivo logado!')
        
    comando = payload.get('comando')
    local_emergencia = payload.get('local')
    room_number = payload.get('room_number')


    if local_topic == 'posto_enfermagem' and comando == 'ligar':

        '''
        Laço condicional para registrar chamada no banco de dados
        '''
        document= {
            'dispositivo_id': dispositivo_id,
            'Local': local_emergencia,
            'Enfermaria': room_number,
            'Data': datetime.now()
        }

        result = mongo.insert_document_collection('chamadas', document)
            
        print(result)

    if local_topic == 'enfermagem':
        print(f'Mensagem do tópico: {local_topic}')
    
    mongo.close_connection()
    print()

'''
Callback para conferir se a inscrição em algum tópico foi bem sucedida
'''
def on_subscribe(client, userdata, mid, granted_os, properties=None):
    print('Inscrito com sucesso!')
