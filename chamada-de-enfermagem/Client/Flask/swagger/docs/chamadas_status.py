from flask_restx import Resource, Namespace
from Flask.swagger.mock.mock_examples import status_chamadas, status_chamadas_emergencia, status_chamadas_oscioso
from Flask.swagger.config import api

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