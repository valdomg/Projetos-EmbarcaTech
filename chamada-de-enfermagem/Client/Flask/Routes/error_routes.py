from flask import Blueprint, render_template
import logging

error_bp = Blueprint('error', __name__)

'''
Arquivo para rotas de erros da aplicação
'''

@error_bp.app_errorhandler(401)
def handle_401(e):
    
    logging.error(f'Erro de de autenticação: {e}')

    return render_template('error/401.html')

@error_bp.app_errorhandler(403)
def handle_403(e):

    logging.error(f'Erro de autorização: {e}')
    
    return render_template('403.html')

@error_bp.app_errorhandler(404)
def error_404(e):
    
    logging.error(f'Erro de rota não encontrada: {e}')
    
    return render_template('error/404.html')



@error_bp.app_errorhandler(500)
def handle_500(e):

    logging.error(f'Erro de servidor: {e}')

    return render_template('error/500.html')


