import RoomServices from "../Room/RoomServices.js";
import RoomModel from '../../models/Room.js';

class TemperatureService {
  constructor(temperatureModel) {
    this.temperatureModel = temperatureModel;
    this.roomService = new RoomServices(RoomModel);
  }

  createTemperatureReading = async (data) => {
    try {
      const { microcontrollerId, temperature, humidity } = data;

      if (!microcontrollerId || temperature == null || humidity == null) {
        throw new Error("MQTT: Dados incompletos recebidos.");
      }

      if (typeof temperature !== "number" || typeof humidity !== "number") {
        throw new Error("Temperatura e umidade devem ser números.");
      }

      const room = await this.roomService.getRoomByMicrocontrollerId(microcontrollerId);

      if (!room) {
        throw new Error(`MQTT: Nenhuma sala encontrada com microcontrolador ${microcontrollerId}`);
      }

      const reading = new this.temperatureModel({
        room: room._id,
        temperature,
        humidity
      });

      return await reading.save();
    } catch (error) {
      console.error(error.message);
      throw error;
    }
  }

  getTemperatureReadings = async () => {
    try {
      return await this.temperatureModel.find().populate('room');
    } catch (error) {
      console.error(error.message);
      throw error;
    }
  }

  getTemperatureReadingsByInterval = async (startDate, endDate) => {
    try {
      await this.validateInterval(startDate, endDate);

      return await this.temperatureModel.find({
        timestamp: {
          $gte: new Date(startDate),
          $lte: new Date(endDate)
        }
      }).populate('room');
    } catch (error) {
      console.error(error.message);
      throw error;
    }
  }

  getRoomTemperatureReadings = async (roomId) => {
    try {
      if (!roomId) {
        throw new Error("ID da sala é obrigatório");
      }

      const readings = await this.temperatureModel.find({ room: roomId }).populate('room');

      if (!readings || readings.length === 0) {
        throw new Error("Nenhuma leitura encontrada para a sala");
      }

      return readings;
    } catch (error) {
      console.error(error.message);
      throw error;
    }
  }

  getRoomTemperatureReadingsByInterval = async (roomId, startDate, endDate) => {
    try {
      if (!roomId) {
        throw new Error("ID da sala é obrigatório");
      }

      await this.validateInterval(startDate, endDate);

      const readings = await this.temperatureModel.find({
        room: roomId,
        timestamp: {
          $gte: new Date(startDate),
          $lte: new Date(endDate)
        }
      }).populate('room');

      if (!readings || readings.length === 0) {
        throw new Error("Nenhuma leitura encontrada para a sala no intervalo especificado");
      }

      return readings;
    } catch (error) {
      console.error(error.message);
      throw error;
    }
  }

  getTemperatureReadingById = async (id) => {
    try {
      if (!id) {
        throw new Error("ID inválido");
      }

      const result = await this.temperatureModel.findById(id).populate('room');

      if (!result) {
        throw new Error("Leitura não encontrada");
      }

      return result;
    } catch (error) {
      console.error(error.message);
      throw error;
    }
  }

  deleteTemperatureReading = async (id) => {
    try {
      if (!id) {
        throw new Error("ID inválido");
      }
      const result = await this.temperatureModel.findByIdAndDelete(id);
      if (!result) {
        throw new Error("Leitura não encontrada");
      }
      return result;
    } catch (error) {
      console.error(error.message);
      throw error;
    }
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
