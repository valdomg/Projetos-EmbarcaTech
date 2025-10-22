from flask import Blueprint, jsonify, request
import logging

mapa_bp = Blueprint('mapa', __name__, url_prefix='/api/mapa')

'''
Rota de api para informar qual enfermaria está com emergência
'''

mapa_bp.route('/', methods=['POST'])
def mapa():
    data = request.get_json()

    logging.warning('Chamada em:', data)

    return jsonify({'status', 'ok'}), 200