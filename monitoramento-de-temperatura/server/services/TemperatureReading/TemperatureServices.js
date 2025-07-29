import TemperatureModel from '../../models/Temperature.js';

class TemperatureService {
  constructor(temperatureModel) {
    this.temperatureModel = temperatureModel;
  }

  createTemperatureReading = async (data) => {
    const temperatureReading = new this.temperatureModel({
      temperature: data.temperature,
      room: data.room,
      humidity: data.humidity,
      microcontrollerId: data.microcontrollerId
    });
    return await temperatureReading.save();
  }

  getTemperatureReadings = async () => {
    return await this.temperatureModel.find();
  }

  getTemperatureReadingById = async (id) => {
    return await this.temperatureModel.findById(id);
  }

  deleteTemperatureReading = async (id) => {
    return await this.temperatureModel.findByIdAndDelete(id);
  }

}

export default TemperatureService;
