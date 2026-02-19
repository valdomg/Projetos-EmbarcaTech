import RoomServices from '../Room/RoomServices.js';
import RoomModel from '../../models/Room.js';
import { sendEnvironmentAlertEmail } from './templates/alert.js';
class EmailService {

        constructor() {
                this.roomService = new RoomServices(RoomModel);
        }

        async sendAlertEmail(microcontrollerId, temperature, humidity) {
                const room = await this.roomService.getRoomByMicrocontrollerId(microcontrollerId);
                sendEnvironmentAlertEmail({
                        to: 'caped.2022123tads0063@aluno.ifpi.edu.br',
                        roomName: room.name,
                        temperature,
                        humidity,
                        minTemp: room.minTemperature,
                        maxTemp: room.maxTemperature,
                        minHumidity: room.minHumidity,
                        maxHumidity: room.maxHumidity
                })
        }

}

export default EmailService;