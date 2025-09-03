import RoomServices from "../Room/RoomServices.js";
import RoomModel from '../../models/Room.js';

class TemperatureService {
  constructor(temperatureModel) {
    this.temperatureModel = temperatureModel;
    this.roomService = new RoomServices(RoomModel);
  }

  createTemperatureReading = async (data) => {
    const { microcontrollerId, temperature, humidity } = data;

    if (!microcontrollerId || temperature == null || humidity == null) {
      console.error("MQTT: Dados incompletos recebidos.");
      return;
    }

    const room = await this.roomService.getRoomByMicrocontrollerId(microcontrollerId);

    if (!room) {
      console.error(`MQTT: Nenhuma sala encontrada com microcontrolador ${microcontrollerId}`);
      return;
    }

    const reading = new this.temperatureModel({
      room: room._id,
      temperature,
      humidity
    });

    return await reading.save();
  }

  getTemperatureReadings = async () => {
    return await this.temperatureModel.find().populate('room');
  }

  getTemperatureReadingsByInterval = async (startDate, endDate) => {

    await this.validateInterval(startDate, endDate);

    return await this.temperatureModel.find({
      timestamp: {
        $gte: new Date(startDate),
        $lte: new Date(endDate)
      }
    });
  }

  getRoomTemperatureReadings = async (roomId) => {
    const room = await this.temperatureModel.find({ room: roomId });

    if (!room) {
      throw new Error("Sala não encontrada");
    }

    return room;
  }

  getRoomTemperatureReadingsByInterval = async (roomId, startDate, endDate) => {
    await this.validateInterval(startDate, endDate);


    return await this.temperatureModel.find({
      room: roomId,
      timestamp: {
        $gte: new Date(startDate),
        $lte: new Date(endDate)
      }
    });
  }

  getTemperatureReadingById = async (id) => {
    if (!id) {
      throw new Error("ID inválido");
    }

    const result = await this.temperatureModel.findById(id).populate('room');

    if (!result) {
      throw new Error("Leitura não encontrada");
    }

    return result;
  }

  deleteTemperatureReading = async (id) => {
    const result = await this.temperatureModel.findByIdAndDelete(id);
    if (!result) {
      throw new Error("Leitura não encontrada");
    }
    return result;
  }

  validateInterval = async (startDate, endDate) => {
    if (!startDate || !endDate) {
      throw new Error("Ambos os campos de data são obrigatórios");
    }

    if (startDate > endDate) {
      throw new Error("Intervalo de datas inválido");
    }

    if (endDate > new Date()) {
      throw new Error("Data final não pode ser no futuro");
    }
    return true;
  }

}

export default TemperatureService;
