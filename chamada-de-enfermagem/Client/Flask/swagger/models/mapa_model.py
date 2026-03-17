from flask_restx import fields
from Flask.swagger.config import api


mapa_payload = api.model(
    'Mapa update emergency',
    {
        'device':       fields.String(description='Nome do dispositivo'),
        'room_number':  fields.String(descriptions='Número da sala'),
        'local':        fields.String(description='Local do dispositivo')
    }
)

mapa_send_message_response = api.model(
    'Mapa model response',
    {
        'Message':  fields.String,
    },
)