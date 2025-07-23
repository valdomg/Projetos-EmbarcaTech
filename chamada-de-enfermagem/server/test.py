from flask import Flask, request
from flask_mqtt import Mqtt

app = Flask(__name__)

# Configuração do broker MQTT
app.config['MQTT_BROKER_URL'] = 'test.mosquitto.org'  # ou IP do seu broker
app.config['MQTT_BROKER_PORT'] = 1883
app.config['MQTT_USERNAME'] = ''  # se necessário
app.config['MQTT_PASSWORD'] = ''  # se necessário
app.config['MQTT_KEEPALIVE'] = 60
app.config['MQTT_TLS_ENABLED'] = False

mqtt = Mqtt(app)

# Subscreve ao tópico após conectar
@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
    print("Conectado ao broker MQTT com código:", rc)
    mqtt.subscribe('teste/topico')

# Quando uma mensagem for recebida
@mqtt.on_message()
def handle_mqtt_message(client, userdata, message):
    payload = message.payload.decode()
    print(f'Mensagem recebida no tópico {message.topic}: {payload}')

# Rota HTTP para publicar via MQTT
@app.route('/publicar', methods=['POST'])
def publish_message():
    dados = request.json
    topico = dados.get('topico')
    mensagem = dados.get('mensagem')
    mqtt.publish(topico, mensagem)
    return {'status': 'mensagem publicada'}

# Início da aplicação Flask
if __name__ == '__main__':
    app.run(debug=True)
