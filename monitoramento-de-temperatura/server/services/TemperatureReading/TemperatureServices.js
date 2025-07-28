class TemperatureService {
  constructor(temperatureModel) {
    this.temperatureModel = temperatureModel;
  }

  async createTemperatureReading(data) {
    const temperatureReading = new this.temperatureModel(data);
    return await temperatureReading.save();
  }

  async getTemperatureReadings() {
    return await this.temperatureModel.find();
  }

  async getTemperatureReadingById(id) {
    return await this.temperatureModel.findById(id);
  }

  async deleteTemperatureReading(id) {
    return await this.temperatureModel.findByIdAndDelete(id);
  }

}

export default TemperatureService;
