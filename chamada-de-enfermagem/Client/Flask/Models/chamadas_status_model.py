import logging
class ChamadasStatusModel:
    '''
    Classe para fazer operações com os status de chamadas vindos do MQTT
    '''
    def __init__(self, salas_status:list):
        self.salas_status = salas_status

    def adicionar_sala(self, room_number:int, status:str) -> bool:
        '''
        Função de adicionar salas, caso já exista uma sala o método apenas muda seu status
        '''
        if not self.salas_status:
            self.salas_status.append({'Sala': room_number,'status':status})
            logging.info(f'Sala registrada: {room_number}')
            return True

        operacao = False
        for index in self.salas_status:
            if index['Sala'] == room_number:
                logging.info(f'Sala registrada: {room_number}')
                operacao = False

                if self.mudar_status(room_number, status):
                    logging.info(f'Status da sala {room_number} é: {status}')
                    return True
            else:
                operacao = True
        
        if operacao:
            self.salas_status.append({'Sala': room_number,'status': status})

        return operacao

        '''
        for index in self.salas_status:
            for value in index.values():
                if value == room_number:
                    print(f'Sala existente: {room_number}')

                    if self.mudar_status(room_number, status):
                        return True
                    
                    return False
                else:
                    self.salas_status.append({'Sala': room_number,'status':status})
                    print(f'Sala registrada: {room_number}')
                    return True
        '''
    '''
    def mudar_status(self, room_number:int, status:str ):
        Método para mudar o status de um sala usando for
        
        if not self.salas_status:
            print('Salas não registradas')
            return False
        
        operacao = False
        
        for index in self.salas_status:
            for value in index.values():
                if value == room_number:
                    index['status'] = status
                    print(f'status da sala: {value} em {status}')
                    
                    operacao = True
                    
                if operacao:
                    break
            
            if operacao:
                break

        if not operacao:
            print(f'Sala não encontrada: {3}')
            return False
    '''
        
    def mudar_status(self, room_number:int, status:str):
        '''
        Método para mudar o status de uma sala
        '''
        operacao = False
        for i in self.salas_status:
            if i['Sala'] == room_number:
                i['status'] = status
                logging.info(f'Sala: {room_number} status de: {status}')
                operacao = True

        if not operacao:
            print(f'Sala não encontrada: {room_number}')

        return operacao
        

    def return_salas(self):
        return self.salas_status
    
    def return_salas_status_emergencia(self):
        salas = {}

        if not self.salas_status:
            return False
        
        for index in self.salas_status:
            if index['status'] == 'emergencia':
                salas[index['Sala']] = index['status']
        
        return salas
    
    def return_salas_status_oscioso(self):
        salas = {}

        if not self.salas_status:
            return False
        
        for index in self.salas_status:
            if index['status'] == 'oscioso':
                salas[index['Sala']] = index['status']
        
        return salas


if __name__ == '__main__':

    lista = []

    salas_status = ChamadasStatusModel(lista)

    salas_status.adicionar_sala(1, 'oscioso')
    salas_status.adicionar_sala(2, 'emergencia')
    salas_status.adicionar_sala(3, 'oscioso')
    salas_status.adicionar_sala(4, 'emergencia')
    salas_status.adicionar_sala(3, 'emergencia')

    teste = salas_status.return_salas()
    print(teste)

    salas_status.mudar_status(2, 'emergencia')
    salas_status.mudar_status(1, 'oscioso')
    salas_status.mudar_status(4, 'oscioso')
    #salas_status.mudar_status(3, 'oscioso')
    #salas_status.mudar_status(4, 'emergencia')

    teste = salas_status.return_salas()
    print(teste)

    print(salas_status.return_salas_status_emergencia())
    print(salas_status.return_salas_status_oscioso())