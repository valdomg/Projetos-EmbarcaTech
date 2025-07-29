import TemperatureService from '../services/TemperatureReading/TemperatureServices.js';
import TemperatureModel from '../models/Temperature.js';

class TemperatureController {
  constructor() {
    this.temperatureService = new TemperatureService(TemperatureModel);
  }

  getAllTemperatures = async (req, res) => {
    try {
      const temperatures = await this.temperatureService.getTemperatureReadings();
      res.status(200).json(temperatures);
    } catch (error) {
      console.error('Error fetching temperatures:', error);
      res.status(500).json({ error: 'Error fetching temperatures.' });
    }
  }

  getTemperatureById = async (req, res) => {
    try {
      const { id } = req.params;
      const temperature = await this.temperatureService.getTemperatureReadingById(id);

      if (!temperature) {
        return res.status(404).json({ error: 'Temperature reading not found.' });
      }

      res.status(200).json(temperature);
    } catch (error) {
      console.error('Error fetching temperature by ID:', error);
      res.status(500).json({ error: 'Error fetching temperature.' });
    }
  }

}

export default TemperatureController;