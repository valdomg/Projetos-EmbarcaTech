from flask_restx import Resource, Namespace
from Flask.swagger.mock.mock_examples import status_chamadas, status_chamadas_emergencia, status_chamadas_oscioso, stop_emergency_response
from Flask.swagger.config import api

from Flask.swagger.models.mapa_model import mapa_payload, mapa_send_message_response

ns_mapa = Namespace('mapa', description='Mapa info')

@ns_mapa.route('/status')
class ChamadasListResource(Resource):
    @ns_mapa.doc('get_status')
    @ns_mapa.response(200, '')
    def get(self):
        '''Obtém a lista de status de chamadas'''
        return{
            'data': status_chamadas
        }
    
@ns_mapa.route('/status/oscioso')
class StatusChamadasListOscioso(Resource):
    @ns_mapa.doc('get_status_oscioso')
    @ns_mapa.expect(200,'')
    def get(self):
        '''Obtém os status de chamadas osciosas'''
        return {
            'data': status_chamadas_oscioso
        }
    
@ns_mapa.route('/status/emergencia')
class StatusChamadasListEmergencia(Resource):
    @ns_mapa.doc('get_status_emergencia')
    @ns_mapa.expect(200,'')
    def get(self):
        '''Obtém os status de chamadas de emergencia'''
        return {
            'data': status_chamadas_emergencia
        }
    
@ns_mapa.route('/mensagem/ocioso')
class StatusChamadasListEmergencia(Resource):
    @ns_mapa.doc('post_mensagem_ocioso')
    @ns_mapa.expect(mapa_payload)
    @ns_mapa.marshal_with(mapa_send_message_response)
    def post(self):
        '''Envia um requisição para encerrar o chamado de um local'''
        data = {
            'device': ns_mapa.payload['device'],
            'room_number': ns_mapa.payload['room_number'],
            'local': ns_mapa.payload['local']
        }
        return {
            'status_code': 200,
            'Message': 'Mensagem entregue ao destinatário',
            'data': data
}