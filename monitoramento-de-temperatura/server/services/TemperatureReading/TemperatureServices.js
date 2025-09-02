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
    return await this.temperatureModel.find({
      timestamp: {
        $gte: new Date(startDate),
        $lte: new Date(endDate)
      }
    });
  }

  getRoomTemperatureReadings = async (roomId) => {
    return await this.temperatureModel.find({ room: roomId });
  }

  getRoomTemperatureReadingsByInterval = async (roomId, startDate, endDate) => {
    return await this.temperatureModel.find({
      room: roomId,
      timestamp: {
        $gte: new Date(startDate),
        $lte: new Date(endDate)
      }
    });
  }

  getTemperatureReadingById = async (id) => {
    return await this.temperatureModel.findById(id).populate('room');
  }

  deleteTemperatureReading = async (id) => {
    return await this.temperatureModel.findByIdAndDelete(id);
  }

}

export default TemperatureService;
