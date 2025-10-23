from flask_restx import fields
from Flask.swagger.config import api

user_payload = api.model(
    'User Register Model',
    {
        'username':     fields.String(description='nome do usuário'),
        'password':     fields.String(description='Senha do usuário'),
        'role':         fields.String(description='nível de acesso do usuário')
    },
)

user_response = api.model(
    'User Response Model',
    {
        'Message':      fields.String,
    },
)

user_payload_delete = api.model( 
    'User Delete Model',
    {
        'document_id': fields.String(description='id do usuário no banco de dados')
    }
)

user_payload_update = api.model( 
    'User Update Model',
    {
        'document_id': fields.String(description='id do usuário no banco de dados'),
        'username': fields.String(description='novo nome do usuário'),
        'password': fields.String(description='nova senha'),
        'role': fields.String(description='novo nível de acesso do usuário')

    }
)