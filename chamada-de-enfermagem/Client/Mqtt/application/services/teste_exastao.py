import threading
import time
import random
import json
import paho.mqtt.client as mqtt

BROKER = "" #endereço do broker
PORT = 1883
NUM_DEVICES = 19 #mudar quantos dispositos quiser



def on_message(client, userdata, msg):
    client_id = client._client_id.decode()
    raw_payload = msg.payload.decode()

    # Tentando converter para JSON com segurança
    try:
        payload = json.loads(raw_payload)
    except json.JSONDecodeError:
        print(f"[{client_id}] ⚠️ Payload não é JSON, ignorando")
        return

    if msg.topic.startswith("dispositivos/enfermaria/"):
        print(f"[{client_id}] Mensagem recebida em {msg.topic}: {payload}")

        resposta = {
            "status": "ok",
            "id":client_id
        }

        client.publish(
            "dispositivo/confirmacao/posto_enfermaria",
            json.dumps(resposta)
        )

        client.publish(
            f"dispositivos/posto_enfermaria/{client_id}",
            "",
            retain = True
        )


    elif msg.topic.startswith("dispositivos/confirmacao/"):
        print(f"[{client_id}] Confirmação recebida: {payload}")
   


def on_connect(client, userdata, flags, rc):
    print("Conectado ao broker! Código:", rc)
    


def simulate_device(device_id):
    client_id = f"enfermaria{device_id}"
    TOPIC_PRINCIPAL_SUB = f"dispositivos/enfermaria/{client_id}"  # tópico que você vai ouvir
    # TOPIC_CONFIRMACAO_PUB = "dispositivo/confirmacao/posto_enfermaria"    # tópico que você vai enviar resposta
    TOPIC_PRINCIPAL_PUB = f"dispositivos/posto_enfermaria/{client_id}"
    TOPIC_CONFIRMACAO_SUB = f"dispositivo/confirmacao/{client_id}"

    client = mqtt.Client(client_id=client_id)
    client.on_message = on_message
    client.username_pw_set(client_id,"hsc@device.com") #colocar senha dos dispositivos
    client.connect(BROKER, PORT, keepalive=60)
    client.subscribe(TOPIC_PRINCIPAL_SUB)
    client.subscribe(TOPIC_CONFIRMACAO_SUB)

    # Loop MQTT em background (thread interna da lib)
    client.loop_start()

    # while True:

    payload = {
        "id": client_id,
        "estado": "emergencia",
        "mensagem": "Ligar LED",
        "room_number": device_id,
        "local": "Enfermaria",
        "comando": "ligar"
    }

    client.publish(TOPIC_PRINCIPAL_PUB, json.dumps(payload), qos=0,retain= True)
    print(f"[{client_id}] Publicado: {payload}")

    time.sleep(random.randint(2, 5))

# Criando threads (dispositivos)
threads = []

for i in range(1,NUM_DEVICES+1):
    t = threading.Thread(
        target=simulate_device,
        args=(i,),
        daemon=True   # encerra quando o programa principal encerra
    )
    t.start()
    threads.append(t)

print("Simulação iniciada...")
while True:
    time.sleep(1)
