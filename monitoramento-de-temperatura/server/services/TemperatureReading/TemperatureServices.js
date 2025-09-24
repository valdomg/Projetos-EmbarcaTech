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
      throw new Error("MQTT: Dados incompletos recebidos.");
    }

    if (typeof temperature !== "number" || typeof humidity !== "number") {
      console.error("MQTT: Temperatura e umidade devem ser números.");
      throw new Error("MQTT: Temperatura e umidade devem ser números.");
    }

    const room = await this.roomService.getRoomByMicrocontrollerId(microcontrollerId);

    if (!room) {
      console.error(`MQTT: Nenhuma sala encontrada com microcontrolador ${microcontrollerId}`);
      throw new Error(`MQTT: Nenhuma sala encontrada com microcontrolador ${microcontrollerId}`);
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

    const start = new Date(startDate);
    const end = new Date(endDate);
    console.log(start, end);

    return await this.temperatureModel.find({
      timestamp: {
        $gte: start,
        $lte: end
      }
    }).populate('room');
  }

  getRoomTemperatureReadings = async (roomId) => {
    if (!roomId) {
      throw new Error("ID da sala é obrigatório");
    }

    const readings = await this.temperatureModel.find({ room: roomId }).populate('room');

    if (!readings || readings.length === 0) {
      throw new Error("Nenhuma leitura encontrada para a sala");
    }

    return readings;
  }

  getRoomTemperatureReadingsByInterval = async (roomId, startDate, endDate) => {
    if (!roomId) {
      throw new Error("ID da sala é obrigatório");
    }

    await this.validateInterval(startDate, endDate);
    const start = new Date(startDate);
    const end = new Date(endDate);
    start.setHours(0, 0, 0, 0);
    end.setHours(23, 59, 59, 999);

    const readings = await this.temperatureModel.find({
      room: roomId,
      timestamp: {
        $gte: start,
        $lte: end
      }
    }).populate('room');

    if (!readings || readings.length === 0) {
      throw new Error("Nenhuma leitura encontrada para a sala no intervalo especificado");
    }

    return readings;
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
    if (!id) {
      throw new Error("ID inválido");
    }
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

    const start = new Date(startDate);
    const end = new Date(endDate);

    if (isNaN(start.getTime()) || isNaN(end.getTime())) {
      throw new Error("Datas inválidas");
    }

    if (start > end) {
      throw new Error("Intervalo de datas inválido");
    }

    if (end > new Date()) {
      throw new Error("Data final não pode ser no futuro");
    }
    return true;
  }

}

export default TemperatureService;
