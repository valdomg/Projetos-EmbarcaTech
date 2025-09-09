import TemperatureService from '../services/TemperatureReading/TemperatureServices.js';
import TemperatureModel from '../models/Temperature.js';
class TemperatureController {
  constructor() {
    this.temperatureService = new TemperatureService(TemperatureModel);
  }

  getAllTemperatures = async (req, res) => {
    const temperatures = await this.temperatureService.getTemperatureReadings();
    res.status(200).json(temperatures);
  }

  getTemperatureById = async (req, res) => {
    const { id } = req.params;
    const temperature = await this.temperatureService.getTemperatureReadingById(id);

    if (!temperature) {
      return res.status(404).json({ error: 'Temperature reading not found.' });
    }

    res.status(200).json(temperature);
  }

  getRoomTemperatures = async (req, res) => {
    const { roomId } = req.params;
    const temperatures = await this.temperatureService.getRoomTemperatureReadings(roomId);
    res.status(200).json(temperatures);
  }

  getRoomTemperaturesByInterval = async (req, res) => {
    const { roomId } = req.params;
    const { startDate, endDate } = req.query;
    const temperatures = await this.temperatureService.getRoomTemperatureReadingsByInterval(roomId, startDate, endDate);
    res.status(200).json(temperatures);
  }

  getTemperatureReadingsByInterval = async (req, res) => {
    const { startDate, endDate } = req.query;
    const temperatures = await this.temperatureService.getTemperatureReadingsByInterval(startDate, endDate);
    res.status(200).json(temperatures);
  }

}

export default TemperatureController;