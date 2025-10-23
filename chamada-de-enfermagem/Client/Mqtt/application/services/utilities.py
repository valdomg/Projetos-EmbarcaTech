import requests
import logging

API_URL = 'http://localhost:5000/api/mapa'
def send_update_to_flask(room_number:int, estado:str):
    '''
    Função para enviar estado do microcontrolador para o endpoint de estados
    '''

    try:
        endpoint = None

        if estado == 'emergencia':
            endpoint = f"{API_URL}/emergencia"

        elif estado == 'oscioso':
            endpoint = f"{API_URL}/oscioso"
            
        else:
            logging.warning(f'Estado desconhecido: {estado}')
            return

        payload = {'room_number': room_number, 'estado':estado}
        response = requests.post(endpoint, json=payload, timeout=3)

        if response.status_code == 200:
            logging.info(f'Sucesso ao enviar status do quarto {room_number} ({estado}) para o mapa.')
        else:
            logging.warning(f'Erro ao enviar status para o mapa: {response.status_code} - {response.text}')

    except requests.exceptions.RequestException as e:
        logging.error(f'Falha ao comunicar com o Flask: {e}')