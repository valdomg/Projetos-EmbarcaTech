import requests
import logging

API_URL = 'http://localhost:5000/api/mapa'
def send_update_to_flask(room_number:int, status:str):
    '''
    Função para enviar status do microcontrolador para o endpoint de statuss
    '''
    try:
        endpoint = None

        if status != 'emergencia' and status != 'oscioso':
            logging.warning(f'status desconhecido: {status}')
            return

        payload = {'room_number': room_number, 'status':status}
        response = requests.post(endpoint, json=payload, timeout=3)

        if response.status_code == 200:
            logging.info(f'Sucesso ao enviar status do quarto {room_number} ({status}) para o mapa.')
        else:
            logging.warning(f'Erro ao enviar status para o mapa: {response.status_code} - {response.text}')

    except requests.exceptions.RequestException as e:
        logging.error(f'Falha ao comunicar com o Flask: {e}')