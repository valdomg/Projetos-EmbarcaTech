import json
import paho.mqtt.client as mqtt
from time import sleep

BROKER_HOST = "localhost"
BROKER_PORT = 1883
id_test = "pub_test"
pass_test = "123"

def publish_message(topic: str, payload: dict):
    """Publica uma mensagem MQTT no tópico especificado."""
    client = mqtt.Client(client_id=id_test)
    client.username_pw_set(id_test, pass_test)
    client.connect(BROKER_HOST, BROKER_PORT)

    payload_f = json.dumps(payload, indent=4) 
    print(payload_f)

    client.publish(topic, payload_f)
    client.disconnect()
    print(f'Mensagem publicada em {topic}: {payload}')

def pub_posto_enfermaria_de_enfermaria(dispositivo_id: str, estado: str, mensagem: str, room_number: str, local:str, comando: str):
    """Publica um evento vindo da enfermaria."""
    topic = f"dispositivos/posto_enfermaria"

    payload = {
        "id": dispositivo_id,
        "estado": estado,
        "mensagem": mensagem,
        "room_number": room_number,
        "local": local,
        "comando": comando
    }

    publish_message(topic, payload)

def pub_enfermaria_de_posto_enfermaria(dispositivo_topic:str, dispositivo_id: str, estado: str, mensagem: str, room_number: str, local:str, comando: str):
    """Publica um evento vindo do posto de enfermaria no tópico local."""
    topic = f"dispositivos/enfermaria/{dispositivo_topic}"

    payload = {
        "id": dispositivo_id,
        "estado": estado,
        "mensagem": mensagem,
        "room_number": room_number,
        "local": local,
        "comando": comando
    }

    publish_message(topic, payload)

def pub_confirmacao_devices(dispositivo_topic:str, status:str):
    """Publica um evento vindo do posto de enfermaria no tópico local."""
    topic = f"dispositivos/confirmacao/{dispositivo_topic}"

    payload = {
        'status': status
    }

    publish_message(topic, payload)
# ====== EXEMPLOS DE USO ======

pub_posto_enfermaria_de_enfermaria(

        dispositivo_id="enfermaria5",
        estado="emergencia",
        mensagem="ligar LED",
        room_number="5",
        local="enfermaria",
        comando="ligar"
        
)

sleep(30)

pub_confirmacao_devices(
    dispositivo_topic='enfermaria5',
    status='ok'
)

sleep(30)

pub_enfermaria_de_posto_enfermaria(
        dispositivo_topic="enfermaria5",
        dispositivo_id="posto_enfermaria",
        estado="ocioso",
        mensagem="desligar LED",
        room_number="5",
        local="enfermaria",
        comando="desligar"
)



