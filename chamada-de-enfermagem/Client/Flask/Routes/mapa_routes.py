from flask import Blueprint, jsonify, request
import logging

mapa_bp = Blueprint('mapa', __name__, url_prefix='/api/mapa')

'''
Rota de api para informar qual enfermaria está com emergência

ROTAS
/api/mapa/emergencia
/api/mapa/oscioso
'''

salas_status = {}

@mapa_bp.route('/emergencia', methods=['POST'])
def emergencia():
    data = request.get_json()
    room_number = data['room_number']
    
    logging.warning(f'Emergência no local: {room_number}')

    salas_status[room_number] = 'emergencia'

    return jsonify({'local':room_number}), 200

@mapa_bp.route('/osciosa', methods=['POST'])
def oscioso():
    data = request.get_json()
    room_number = data['room_number']

    logging.warning(f'Osciocidade no local: {room_number}')

    salas_status[room_number] = 'oscioso'

    return jsonify({'local':room_number}), 200

@mapa_bp.route('/status', methods=['GET'])
def get_status():
    print(salas_status)
    return jsonify(salas_status), 200