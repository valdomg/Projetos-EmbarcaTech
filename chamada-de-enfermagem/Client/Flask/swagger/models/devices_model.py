from flask_restx import fields
from Flask.swagger.config import api

device_payload = api.model(
    'Device Register Model',
    {
        'device':     fields.String(description='nome do device'),
    },
)

device_response = api.model(
    'Device Model Response',
    {
        'Message':      fields.String,
    },
)

device_payload_delete = api.model(
    'Device Model Delete',
    {
        'document_id': fields.String(description='id do device no banco de dados')
    }
)

device_payload_update = api.model(
    'Device Model Update',
    {
        'document_id': fields.String(description='id do device no banco de dados'),
        'device': fields.String(description='novo nome do device'),
    }
)