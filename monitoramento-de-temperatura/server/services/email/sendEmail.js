import RoomServices from '../Room/RoomServices.js';
import RoomModel from '../../models/Room.js';
import UserModel from '../../models/User.js';
import { sendEnvironmentAlertEmail } from './templates/alert.js';

class EmailService {

        constructor() {
                this.roomService = new RoomServices(RoomModel);
        }

        async sendAlertEmail(microcontrollerId, temperature, humidity) {
                const room = await this.roomService.getRoomByMicrocontrollerId(microcontrollerId);

                const admins = await UserModel.find({ role: 'admin' });

                if (!admins || admins.length === 0) {
                        console.warn('Nenhum administrador encontrado para envio de alerta.');
                        return;
                }

                const emailPromises = admins.map(admin =>
                        sendEnvironmentAlertEmail({
                                to: admin.email,
                                roomName: room.name,
                                temperature,
                                humidity,
                                minTemp: room.minTemperature,
                                maxTemp: room.maxTemperature,
                                minHumidity: room.minHumidity,
                                maxHumidity: room.maxHumidity
                        })
                );

                const results = await Promise.allSettled(emailPromises);
                results.forEach((result, index) => {
                        if (result.status === 'rejected') {
                                console.error(`Erro ao enviar alerta para ${admins[index].email}:`, result.reason);
                        }
                });
        }

}

export default EmailService;