from flask_restx import Resource, Namespace
from Flask.swagger.mock.mock_examples import devices
from Flask.swagger.config import api

from Flask.swagger.models.devices_model import device_payload, device_response, device_payload_update, device_payload_delete


ns_device = Namespace('devices', description='Devices operations')

@ns_device.route('')
class DeviceListResource(Resource):
    @ns_device.doc('return_all_devices')
    @ns_device.response(200, '')
    def get(self):
        '''Obtém a lista de devices'''
        return{
            'data': devices
        }
    
@ns_device.route('/register')
class DevocePostResource(Resource):
    @ns_device.doc('post_a_device')
    @ns_device.expect(device_payload)
    @ns_device.marshal_with(device_response)
    def post(self):
        '''Insere um novo device na base de dados'''
        data = {
            'device': ns_device.payload['device'], 
        }

        return {
                'status_code': 201,
                'Message':'Device cadastrado com sucesso',
                'data': data
            }
    
@ns_device.route('/delete')
class DeviceDeleteResource(Resource):
    @ns_device.doc('delete_a_device')
    @ns_device.expect(device_payload_delete)
    @ns_device.marshal_with(device_response)
    def delete(self):
        '''Exclui um device da database'''
        data = {
            'document_id': ns_device.payload['document_id']
        }

        return {
            'status_code': 200,
            'Message': 'Device deletado com sucesso'
        }
    
@ns_device.route('/update')
class DeviceUpdateResource(Resource):
    @ns_device.doc('update_a_device')
    @ns_device.expect(device_payload_update)
    @ns_device.marshal_with(device_response)
    def put(self):
        '''Atualiza um device'''
        data = {
            'document_id': ns_device.payload['document_id']
        }

        return {
            'status_code': 200,
            'Message': 'Dispositivo atualizado'
        }