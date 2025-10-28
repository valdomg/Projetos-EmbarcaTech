from flask import Flask
from Flask.swagger.config import api
from Flask.swagger.docs.users_doc import ns_user
from Flask.swagger.docs.devices_doc import ns_device
from Flask.swagger.docs.chamadas_doc import ns_chamadas
from Flask.swagger.docs.chamadas_status import ns_mapa



doc = Flask(__name__)

api.init_app(doc)
api.add_namespace(ns_user, path='/api/users')
api.add_namespace(ns_device, path='/api/devices')
api.add_namespace(ns_chamadas, path='/api/chamadas')
api.add_namespace(ns_mapa , path='/api/mapa')


if __name__ == '__main__':
    doc.run(host='0.0.0.0', port=5001, debug=True)

