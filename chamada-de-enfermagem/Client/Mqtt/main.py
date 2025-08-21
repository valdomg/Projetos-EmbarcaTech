
from application.controller.mqtt_client_connection import MqttClientConnection
import os
from dotenv import load_dotenv

load_dotenv()

broker = os.getenv('BROKER_IP')
port = int(os.getenv('BROKER_PORT'))
topic = os.getenv('BROKER_TOPIC')
client_name = os.getenv('CLIENT_NAME')

print(broker, port, topic)

if __name__ == '__main__':
    mqttclient = MqttClientConnection(broker, port, client_name, 60)
    mqttclient.start()
    