from flask import Blueprint, request, render_template, redirect, make_response, url_for
import jwt
from datetime import datetime, timedelta, timezone
from Flask.Services.user_service import UserService
from Flask.Models.user_db_model import UserDBModel
from Flask.auth import SECRET_KEY
from MongoDB.mongo_conn import mongo_conn
import logging

'''
Arquivo para rotas do client
'''

user_db_model = UserDBModel(mongo_conn)
user_service = UserService(user_db_model)

auth_bp = Blueprint('auth', __name__, url_prefix='/auth')

'''
ROTAS

/auth/login
/auth/logout
'''

'''
Rota de login
POST para receber as informações do usuário por meio de um formulário

retorna um token para acessar o sistema ou uma mensagem de credenciais inválidas
'''
@auth_bp.route('/login', methods=['GET', 'POST'])
def login():

    error=None

    if request.method == 'POST':

        try:
            username = request.form.get('username')
            password = request.form.get('password')

            resp = user_service.login(username, password)
            
            if resp[2] != None:

                user = resp[2]

                token = jwt.encode({
                    'user'  :   user['username'],
                    'role'  :   user['role'],
                    'exp'   :   datetime.now(timezone.utc) + timedelta(minutes=10)
                }, SECRET_KEY, algorithm='HS256')

                resp = make_response(redirect(url_for('pages.relatorio_page')))
                resp.set_cookie('jwt', token, httponly=True)

                logging.info('Usuário logado!')

                return resp

            error ='Credenciais inválidas'
        
        except Exception as e:
            logging.exception(e)
            error= 'Erro ao processar login'

    return render_template('login.html', error=error)

'''
Função para deslogar usuário

retorna a página de login
'''
@auth_bp.route('/logout')
def logout():

    resp = make_response(redirect(url_for('auth.login')))
    
    resp.set_cookie('jwt', '', expires=0, httponly=True)

    return resp