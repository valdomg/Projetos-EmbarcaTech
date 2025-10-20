from flask_restx import Resource, Namespace
from Flask.swagger.mock.mock_examples import users
from Flask.swagger.config import api

from Flask.swagger.models.users_model import user_payload, user_response, user_payload_delete, user_payload_update

ns_user = Namespace('users', description='Users operations')

@ns_user.route('')
class UserListResource(Resource):
    @ns_user.doc('return_all_users')
    @ns_user.response(200, '')
    def get(self):
        '''Obtém a lista de users'''
        return{
            'data': users
        }
    
@ns_user.route('/register')
class UserPostResource(Resource):
    @ns_user.doc('post_a_user')
    @ns_user.expect(user_payload)
    @ns_user.marshal_with(user_response)
    def post(self):
        '''Insere um novo usuário na base de dados'''
        data = {
            'username': ns_user.payload['username'],
            'password': ns_user.payload['password'],
            'role': ns_user.payload['role'] 
        }

        return {
                'status_code': 200,
                'Message':'Usuário cadastrado com sucesso',
                'data': data
            }
    
@ns_user.route('/delete')
class UserDeleteResource(Resource):
    @ns_user.doc('delete_a_user')
    @ns_user.expect(user_payload_delete)
    @ns_user.marshal_with(user_response)
    def delete(self):
        '''Exclui um usuário da database'''
        data = {
            'document_id': ns_user.payload['document_id']
        }

        return {
            'status_code': 200,
            'Message': 'Usuário deletado com sucesso'
        }
    
@ns_user.route('/update')
class UserUpdateResource(Resource):
    @ns_user.doc('update_a_user')
    @ns_user.expect(user_payload_update)
    @ns_user.marshal_with(user_response)
    def put(self):
        '''Atualiza um ou mais campos do usuário'''
        data = {
            'document_id':  ns_user.payload['document_id'],
            'username':     ns_user.payload['username'],
            'password':     ns_user.payload['password'],
            'role':     ns_user.payload['role'],
        }

        return {
            'status_code':201,
            'Message': 'campos atualizados!'
        }