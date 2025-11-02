from flask_restx import Resource, Namespace
from Flask.swagger.mock.mock_examples import chamadas
from Flask.swagger.config import api

ns_chamadas = Namespace('chamadas', description='Chamadas operations')

@ns_chamadas.route('/')
class ChamadasListResource(Resource):
    @ns_chamadas.doc('return_all_chamadas')
    @ns_chamadas.response(200, '')
    def get(self):
        '''Obtém a lista de chamadas'''
        return{
            'data': chamadas
        }
    
@ns_chamadas.route('/dia/contagem')
class ChamadasListDayCount(Resource):
    @ns_chamadas.doc('return_chamadas_count_day')
    @ns_chamadas.expect(len(chamadas))
    def get(self):
        '''Obtém contagem de chamadas do dia'''
        return {
            'Quantidade': len(chamadas)
        }
    
@ns_chamadas.route('/todas/contagem')
class ChamadasListAllCountResource(Resource):
    @ns_chamadas.doc('return_chamadas_count_all')
    @ns_chamadas.expect(len(chamadas))
    def get(self):
        '''Retorna a quantidade de chamadas no banco de dados'''

        return {                
                'Quantidade': len(chamadas)
            }
    
@ns_chamadas.route('/dia')
class ChamadasListDayResource(Resource):
    @ns_chamadas.doc('delete_a_chamadas')
    @ns_chamadas.expect(len(chamadas))
    def get(self):
        '''Exclui um chamadas da database'''
        return {
            'Quantidade': len(chamadas)
        }