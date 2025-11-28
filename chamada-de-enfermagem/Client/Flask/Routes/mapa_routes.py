
from flask import Blueprint, jsonify, request
import logging
from dotenv import load_dotenv
import os
from Flask.auth import token_required, admin_required
from Flask.Models.chamadas_status_model import ChamadasStatusModel
from Flask.Services.convert_objectdID import convert_all_id_to_string, convert_object_id_to_string
from MongoDB.MongoDBConnection import MongoDBConnection


mapa_bp = Blueprint('mapa', __name__, url_prefix='/api/mapa')
'''
Rota de api para informar status das enfermaria

ROTAS
/api/mapa/status/oscioso
/api/mapa/status/emergencia
/api/mapa/status
'''
load_dotenv()

uri = os.getenv('MONGO_URI')
database = os.getenv('MONGO_DATABASE')

mongo_conn = MongoDBConnection(uri, database)
status_chamadas = ChamadasStatusModel(mongo_conn)


@mapa_bp.route('/status', methods=['GET'])
@token_required
def get_status():

    try:
        mongo_conn.start_connection()
    except Exception as e:
        logging.exception('Erro ao conectar ao banco de dados')
        return jsonify({'Error':'Erro interno do banco de dados'}), 500

    try:
        chamadas = status_chamadas.return_docs_from_status_chamadas()

        if chamadas:
            json_chamadas = convert_all_id_to_string(chamadas)
            return jsonify(json_chamadas), 200

    except Exception as e:
        logging.exception('Error in return documents')
        return jsonify({'Error':'failure in return documents'}), 400

    finally:
        mongo_conn.close_connection()

    return {'Error': 'Chamadas não encontradas'},404


@mapa_bp.route('/status/oscioso', methods=['GET'])
@token_required
def get_status_oscioso():

    try:
        mongo_conn.start_connection()
    except Exception as e:
        logging.exception('Erro ao conectar ao banco de dados')
        return jsonify({'Error':'Erro interno do banco de dados'}), 500

    try:
        chamadas = status_chamadas.return_docs_status('oscioso')

        if chamadas:
            json_chamadas = convert_all_id_to_string(chamadas)
            return jsonify(json_chamadas), 200

    except Exception as e:
        logging.exception('Error in return documents')
        return jsonify({'Error':'failure in return documents'}), 400

    finally:
        mongo_conn.close_connection()

    return {'Error': 'Chamadas não encontradas'},404

@mapa_bp.route('/status/emergencia', methods=['GET'])
@token_required
def get_status_emergencia():

    try:
        mongo_conn.start_connection()
    except Exception as e:
        logging.exception('Erro ao conectar ao banco de dados')
        return jsonify({'Error':'Erro interno do banco de dados'}), 500

    try:
        chamadas = status_chamadas.return_docs_status('emergencia')

        if chamadas:
            json_chamadas = convert_all_id_to_string(chamadas)
            return jsonify(json_chamadas), 200

    except Exception as e:
        logging.exception('Error in return documents')
        return jsonify({'Error':'failure in return documents'}), 400

    finally:
        mongo_conn.close_connection()

    return {'Error': 'Chamadas não encontradas'},404

