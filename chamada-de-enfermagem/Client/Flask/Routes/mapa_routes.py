from flask import Blueprint, jsonify, request
import logging
from Flask.Models.chamadas_status_model import ChamadasStatusModel

mapa_bp = Blueprint('mapa', __name__, url_prefix='/api/mapa')

'''
Rota de api para informar qual enfermaria está com emergência

ROTAS
/api/mapa/registrar_chamadas
/api/mapa/status/oscioso
/api/mapa/status/emergencia
/api/mapa/status
'''

status_chamadas = ChamadasStatusModel(list())

@mapa_bp.route('/registrar', methods=['POST'])
def registrar_chamada():

    data = request.get_json()
    try:
        if 'room_number' not in data or 'status' not in data:
            return jsonify({'Erro':'Campos incorretos'}), 400
        
        if status_chamadas.adicionar_sala(data['room_number'], data['status']):
            return jsonify({'Message': 'chamado registrado'}), 201

    except Exception as e:
        print(e)

@mapa_bp.route('/status', methods=['GET'])
def get_status():
    print(status_chamadas.return_salas())
    return jsonify(status_chamadas.return_salas()), 200


@mapa_bp.route('/status/oscioso', methods=['GET'])
def get_status_oscioso():
    salas = status_chamadas.return_salas_status_oscioso()
    return jsonify(salas), 200

@mapa_bp.route('/status/emergencia', methods=['GET'])
def get_status_emergencia():
    salas = status_chamadas.return_salas_status_emergencia()
    return jsonify(salas), 200



