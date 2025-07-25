from flask import Flask, request
from flask_mqtt import Mqtt

app = Flask(__name__)


'''
Configurações para o Client
'''
app.config['MQTT_BROKER_URL']       =       ''  #url do broker/endereço ip
app.config['MQTT_BROKER_PORT']      =       ''
app.config['MQTT_USERNAME']         =       '' 
app.config['MQTT_PASSWORD']         =       ''
app.config['MQTT_KEEPALIVE']        =       60
app.config['MQTT_TLS_ENABLE']       =       False

topic = 'teste/topico'                          #topico para teste

mqtt = Mqtt(app)

'''
Conexão em um tópico [inscrição]
'''
@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
    print("Conectado ao broker MQTT Local com código:", rc)
    mqtt.subscribe(topic)

'''
Lidando com mensagens [inscrição]
'''
@mqtt.on_message()
def handle_mqtt_message(client, userdata,message):
    payload = message.payload.decode()
    print(f'Mensagem recebida no tópico {message.topic}: {payload}')


'''
Publicando uma mensagem no tópico 
'''
@app.route('/publicar', methods=['POST'])
def publish_message():
    data = request.json
    topic = data.get('topic')
    message = data.get('message')
    mqtt.publish(topic, message)
    return {'status', 'mensagem recebida'}

'''
Desiscrevendo de um tópico
'''
def unsubscribe_topic(topic):
    mqtt.unsubscribe(topic)


'''
Conexão com o MongoDB
'''


'''
Rotas/tópicos para cada microcontrolador
'''
@app.route("/")
def hello_world():
    return "<p>Hello, World!</p>"

'''
Rota/tópico para o microcontrolador do Posto de Enfermagem
'''

'''
Rota para página de login
'''

'''
Rota para página de relatório
'''

'''
Rota para salvar o chamado no BD
'''


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)