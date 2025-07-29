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

  getTemperatureReadingsByInterval = async (startDate, endDate) => {
    return await this.temperatureModel.find({
      createdAt: {
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
    return await this.temperatureModel.findById(id);
  }

  deleteTemperatureReading = async (id) => {
    return await this.temperatureModel.findByIdAndDelete(id);
  }

}

export default TemperatureService;
