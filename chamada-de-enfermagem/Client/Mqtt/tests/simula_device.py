import paho.mqtt.client as mqtt
from time import sleep
import json
import logging
from random import randint
import threading

name_client = 'enfermaria2'
room_number = 1
local = 'Bloco A'

broker = '3.144.223.37'
port = 1883
password = 'hsc@device.com'

topic_confirm = f'dispositivos/confirmacao/{name_client}'
topic_posto = f'dispositivos/posto_enfermaria/{name_client}'
topic_atendimento = f'dispositivos/atendimento/{name_client}'

dispositivos = []

class MqttClientConnection:
    '''
    Classe de conexão do mqtt

    atributos

    broker          = ip do broker
    port            = porta do broker
    client_name     = nome do client
    user_name       = nome de usuário para a senha
    password        = senha para o client
    keepalive       = tempo para verificar o estado do broker (ativo ou não)
    '''
    
    def __init__(self, broker_ip:str, port:int, client_name:str, user_name:str, password:str ,keepalive:int, local:str, room_number:int):
        self.__broker_ip      =       broker_ip
        self.__port           =       port
        self.__client_name    =       client_name
        self.__user_name      =       user_name
        self.__password       =       password
        self.__keepalive      =       keepalive
        self.result_dc = None
        
        self.local = local
        self.room_number = room_number

        self.payload_emergencia = {
            "id": self.__client_name,
            "estado": "emergencia",
            "mensagem": "Ligar LED",
            "room_number": self.room_number,
            "local": self.local,
            "comando": "ligar"
        } 
        
        self.payload_ocioso = {
            "id": self.__client_name,
            "estado": "ocioso",
            "mensagem": "Desligar LED",
            "room_number": self.room_number,
            "local": self.local,
            "comando": "desligar"
        }

        self.btn_estado      =       'destravado'

        self.topic_confirm  = f'dispositivos/confirmacao/{self.__client_name}'
        self.topic_posto    = f'dispositivos/posto_enfermaria/{self.__client_name}'
        self.topic_atendimento  = f'dispositivos/atendimento/{self.__client_name}'

        self.__mqtt_client    =       None

    def __start_connection(self) -> bool:
        '''
        Função para conectar o client ao broker, definir as funções de callback
        '''

        try:
            
            mqtt_client = mqtt.Client(
                client_id=self.__client_name, protocol=mqtt.MQTTv5,
                callback_api_version=mqtt.CallbackAPIVersion.VERSION2
            )
            
            if self.__user_name and self.__password:
                mqtt_client.username_pw_set(self.__user_name, self.__password)
                
            mqtt_client.on_connect      = self.on_connect
            mqtt_client.on_message      = self.on_message
            mqtt_client.on_subscribe    = self.on_subscribe
            mqtt_client.on_disconnect   = self.on_disconnect

            mqtt_client.connect_async(host=self.__broker_ip, port=self.__port, keepalive=self.__keepalive)
            self.__mqtt_client = mqtt_client

            self.__mqtt_client.loop_start()

            
        except Exception as e:
            logging.exception(e)
            return False
        
    def start(self):
            
        '''
        Função com loop infinito para manter o client ativo
        '''
        self.__start_connection()
        while True: sleep(0.001)


    def get_client_name(self):
        return self.__client_name

    
    def publish_on_topic(self,topic:str, payload:dict, retain:bool):
        
        payload_to_send = ''

        if payload != None:
            payload_to_send = json.dumps(payload)

        self.__mqtt_client.publish(topic, payload_to_send, retain=retain)

    def on_message(self, client, usardata, message, properties=None):
        
        try:
            payload = json.loads(message.payload.decode())
        except json.JSONDecodeError:
            logging.error('Erro ao abrir JSON')
            return
        
        logging.info(f'Mensagem recebida de {client}')
        logging.info(f'Payload: {payload}')

        partes = message.topic.split('/')

        logging.info(f'Partes: {partes}')

        _, local_topic, device_id_topic = partes

        if local_topic == 'confirmacao':
            self.btn_estado = 'travado'
            logging.info(f'Botão do {self.__client_name} está {self.btn_estado}')

        if local_topic == 'atendimento':
            atendimento = payload.get('atendimento')

            if atendimento == 'atendido':
                self.btn_estado = 'destravado'
                self.publish_on_topic(self.topic_posto, self.payload_ocioso, retain=True)

            if atendimento == 'finalizado':
                self.publish_on_topic(self.topic_posto, payload=None, retain=True)
                sleep(1)
                self.result_dc = self.__mqtt_client.disconnect()
                logging.info(f'Erros na desconexão: {self.result_dc}')    

        if local_topic == 'enfermaria':
            logging.info(f'Mensagem recebida no tópico enfermaria')

    
    def on_connect(self, client, userdata, flags, rc, properties=None):
        '''
        Callback usado ao conectar o client a algum tópico do broker
        '''
        if rc != 0:
            logging.error('Conexão mal sucedida, erro:', rc, '\n Tentando novamente...')
            self.reconnect_mqtt(client)

        self.__mqtt_client.subscribe(self.topic_confirm)
        self.__mqtt_client.subscribe(self.topic_atendimento)
        self.__mqtt_client.subscribe(self.topic_posto)

        self.publish_on_topic(topic=self.topic_posto, payload=self.payload_emergencia, retain=True)


    def on_disconnect(self, client, userdata, flags, rc, properties=None):
        '''
        Callback para quando houver desconexões inesperadas
        '''
        if rc != 0:
            logging.error('Desconexão inesperada, tentando novamente...', rc)

        logging.info('Tentando novamente...')

        if self.result_dc != 0:
            try:
                self.reconnect_mqtt(client)

            except Exception as e:
                logging.exception('Conexão mal sucedida, erro:', rc)

        else:
            logging.info(f'Desconexão estranha...{ self.result_dc}')

    
    def on_subscribe(self, client, userdata, mid, granted_os, properties=None):
        '''
        Callback para conferir s'e a inscrição em algum tópico foi bem sucedida
        '''
        logging.info(f'Inscrito com sucesso no tópico')

    
    def reconnect_mqtt(self, client):
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


if __name__ == '__main__':

    logging.basicConfig(
        level=logging.INFO,
        format='Data: %(levelname)s - %(asctime)s - %(filename)s - %(message)s'
    )

    broker = '3.144.223.37'
    port = 1883
    password = 'hsc@device.com'
    
    locais = ['Bloco A', 'Ala Sul', 'Andar 1', 'Enfermaria 2', 'UTI A']

    num_devices = 40



    for i in range(1, num_devices+1):
        
        device = f'enfermaria{i}'
        local = locais[randint(0, len(locais) - 1)]

        device_mqtt = MqttClientConnection(broker, port, device, device, password, 60, local, i)

        t = threading.Thread(
            target=device_mqtt.start,
            daemon=True
        )

        t.start()
        dispositivos.append(t)

        sleep(0.5)

    while True:
        #print(dispositivos)
        sleep(1)

print(dispositivos)
    