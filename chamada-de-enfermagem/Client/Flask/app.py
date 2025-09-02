from flask import Flask, request, jsonify,render_template
from Mqtt.application.models.MongoDBConnection import MongoDBConnection
from dotenv import load_dotenv
import os

load_dotenv()

app = Flask(__name__)

uri = os.getenv('MONGO_URI')
database = os.getenv('MONGO_DATABASE')
mongo_conn = MongoDBConnection(uri, database)


@app.route('/')
def start():
    mongo_conn.start_connection()
    result = mongo_conn.list_documents('chamadas')
    print(result)
    return render_template('login.html')

@app.route('/home')
def home():
    return render_template('/relatorio.html')

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)