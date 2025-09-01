from flask import Flask, request, jsonify
from Mqtt.application.models.MongoDBConnection import MongoDBConnection
from dotenv import load_dotenv

load_dotenv()

app = Flask(__name__)
mongo_conn = MongoDBConnection('mongodb://localhost:27017/','chamada-enfermagem')

@app.route('/')
def start():
    mongo_conn.start_connection()
    result = mongo_conn.list_documents('chamadas')
    print(result)
    return 'Olá'

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)