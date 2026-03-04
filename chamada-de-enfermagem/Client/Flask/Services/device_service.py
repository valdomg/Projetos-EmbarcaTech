from Flask.Models.device_db_model import DeviceDBModel
from Flask.Models.device_model import Device
from datetime import datetime
from flask import jsonify
from bson.objectid import ObjectId
from datetime import datetime
from Scripts.device_scripts import run_command_to_add_device_mosquitto 
import logging

'''
Classe de utilitário de registro de devices
'''

class DeviceService:
    '''
    Utiliza um DeviceDBModel para realizar as operações de registro
    '''
    def __init__(self, device_db_model:DeviceDBModel):
        self.device_db_model = device_db_model

    '''
    Registra um device com seu nome
    retorna um json com o status da inserção
    '''
    def register(self, data:dict):

        if 'device' not in data or 'room_number' not in data:
            return jsonify({'message': 'Valores errados, verifique as informações de dispositivo e número da sala'}), 400

        device = data['device']
        room_number = data['room_number']

        try:
            if not device or device.split() == '' or ' ' in device:
                return jsonify ({'message': 'Valor de dispositivo errado, verifique as informações'}), 400
            
            if isinstance(room_number, str):
                if not room_number or room_number.isdigit() is False:
                    return jsonify({'message': 'Valor de número errado, verifique as informações'}), 400

        except Exception as e:
            logging.exception(e)
            return jsonify({'message': 'Erro nos dados enviados'}), 500
                    
        temp = int(room_number)
        room_number = temp

        devite_to_register = Device(device, room_number)

        if self.device_db_model.find_by_device(devite_to_register.getDevice()):
            return jsonify({'error': 'Device já existente'}), 400

        if self.device_db_model.insert_device({
            'device': devite_to_register.getDevice(),
            'room_number': devite_to_register.getRoomNumber(),
            'createdAt': devite_to_register.getCreatedAt()
            }) is False:

            return jsonify({'message': 'Device não inserido no banco de dados'}), 500

        run_command_to_add_device_mosquitto(device)
        return jsonify({'message': 'Device cadastrado com sucesso'}), 201
    
    def delete(self, document_id:str):

        if ObjectId.is_valid(document_id) is False: 
            return jsonify({'message':'ID de dispositivo incorreto'}), 404

        if self.device_db_model.find_device_by_id(document_id) is False:
            return jsonify({'message': 'Dispositivo não encontrado'}), 404

        if self.device_db_model.delete_device_by_id(document_id) is False:
            return jsonify({'message': 'dispositivo não deletado'}), 500
        
        return jsonify({'message' : 'Dispositivo deletado!'}) , 200

    def update(self, document_id:str, document_with_updates:dict):
        document_with_updates.pop('document_id')

        if ObjectId.is_valid(document_id) is False: 
            return jsonify({'message':'ID de dispositivo incorreto'}), 404
        
        try:

            if 'room_number' in document_with_updates:
                room_number = document_with_updates['room_number']

                if isinstance(room_number, str):
                    if not room_number or room_number.isdigit() is False:
                        return jsonify({'message': 'Valor de número errado, verifique as informações'}), 400

                document_with_updates['room_number'] = int(room_number)

            if 'device' in document_with_updates:
                device = document_with_updates['device']

                if not device or device.split() == '' or ' ' in device:
                    return jsonify({'message': 'Valores faltosos ou com espaço, verifique as informações'}), 400

                if self.device_db_model.find_device_by_id(document_id) is False:        
                    return jsonify({'message': 'Device não encontrado'}), 400

                if self.device_db_model.find_by_device(device) is True:
                    return jsonify({'message': 'Nome de device em uso, valor não alterado!'}), 400

                run_command_to_add_device_mosquitto(document_with_updates['device'])

        except Exception as e:
            logging.exception(e)
            return jsonify({'Error': 'Verifique os dados'}), 500

        if self.device_db_model.update_device(document_id, document_with_updates) is False:
            return jsonify({'message': 'Device não alterado, tente novamente'}), 500
        
        return jsonify({'Message': 'Dispositivo atualizado!'}), 200