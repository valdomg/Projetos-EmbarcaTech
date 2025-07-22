from flask import Flask

app = Flask(__name__)


'''
Conexão com o MongoDB
'''


'''
Rotas/tópicos para cada microcontrolador
'''
@app.route("/")
def hello_world():
    return "<p>Hello, World!</p>"

'''
Rota/tópico para o microcontrolador do Posto de Enfermagem
'''

'''
Rota para página de login
'''

'''
Rota para página de relatório
'''

'''
Rota para salvar o chamado no BD
'''


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)