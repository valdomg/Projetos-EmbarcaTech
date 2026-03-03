import TemperatureService from '../services/TemperatureReading/TemperatureServices.js';
import TemperatureModel from '../models/Temperature.js';
import { ReportPdfService } from '../services/pdf/generatePdf.js';
class TemperatureController {
  constructor() {
    this.temperatureService = new TemperatureService(TemperatureModel);
  }

  getAllTemperatures = async (req, res) => {
      /*
          #swagger.tags = ['Temperature']
          #swagger.summary = 'Obtém todas as leituras de temperatura'
          #swagger.description = 'Esse endpoint retorna todas as leituras de temperatura registradas no sistema.'
          #swagger.security = [{
            "bearerAuth": []
          }] 
          */

    const temperatures = await this.temperatureService.getTemperatureReadings();
    res.status(200).json(temperatures);

  }

  getTemperatureById = async (req, res) => {
      /*
          #swagger.tags = ['Temperature']
          #swagger.summary = 'Obtém uma leitura de temperatura pelo ID'
          #swagger.description = 'Esse endpoint retorna uma leitura de temperatura específica com base no ID fornecido.'
          #swagger.security = [{
            "bearerAuth": []
          }] 
          */

    const { id } = req.params;
    const temperature = await this.temperatureService.getTemperatureReadingById(id);

    if (!temperature) {
      return res.status(404).json({ error: 'Temperature reading not found.' });
    }

    res.status(200).json(temperature);
  }

  getRoomTemperatures = async (req, res) => {
          /*
          #swagger.tags = ['Temperature']
          #swagger.summary = 'Obtém as leituras de temperatura de um ambiente específico'
          #swagger.description = 'Esse endpoint retorna todas as leituras de temperatura registradas para um ambiente específico com base no ID do ambiente fornecido.'
          #swagger.security = [{
            "bearerAuth": []
          }] 
          */
    const { roomId } = req.params;
    const temperatures = await this.temperatureService.getRoomTemperatureReadings(roomId);
    res.status(200).json(temperatures);
  }

  getRoomTemperaturesByInterval = async (req, res) => {
          /*
          #swagger.tags = ['Temperature']
          #swagger.summary = 'Obtém as leituras de temperatura de um ambiente específico por intervalo'
          #swagger.description = 'Esse endpoint retorna todas as leituras de temperatura registradas para um ambiente específico com base no ID do ambiente e no intervalo de datas fornecidos.'
          #swagger.security = [{
            "bearerAuth": []
          }] 
          */

    const { roomId } = req.params;
    const { startDate, endDate } = req.query;
    const temperatures = await this.temperatureService.getRoomTemperatureReadingsByInterval(roomId, startDate, endDate);
    res.status(200).json(temperatures);
  }

  getCurrentTemperature = async (req, res) => {

          /*
          #swagger.tags = ['Temperature']
          #swagger.summary = 'Obtém a leitura de temperatura mais recente organizada por ambiente'
          #swagger.description = 'Esse endpoint retorna a leitura de temperatura mais recente registrada para cada ambiente.'
          #swagger.security = [{
            "bearerAuth": []
          }] 
          */

      const result = await this.temperatureService.getRoomsWithLastReading();
      res.status(200).json(result);
    }

  getReport = async (req, res) => {
          /*
          #swagger.tags = ['Temperature']
          #swagger.summary = 'Obtém as leituras de temperatura e humidade por hora de um ambiente específico por intervalo'
          #swagger.description = 'Esse endpoint retorna media, minima e maxima de temperatura e humidade por hora para um ambiente específico com base no ID do ambiente e no intervalo de datas fornecidos.'
          #swagger.security = [{
            "bearerAuth": []
          }] 
          */

    const { roomId } = req.params;
    const { startDate, endDate } = req.query;
    const result = await this.temperatureService.getReport(roomId, startDate, endDate);
    res.status(200).json(result);
  }

  getPdfReport = async (req, res) => {

          /*
          #swagger.tags = ['Temperature']
          #swagger.summary = 'Gera um relatório em PDF das leituras de temperatura e humidade por hora de um ambiente específico por intervalo'
          #swagger.description = 'Esse endpoint gera um relatório em PDF contendo media, minima e maxima de temperatura e humidade por hora para um ambiente específico com base no ID do ambiente e no intervalo de datas fornecidos. O PDF é gerado utilizando o serviço ReportPdfService e retorna um link para download do arquivo.'
          #swagger.security = [{
            "bearerAuth": []
          }] 
          */

    const { roomId } = req.params;
    const { startDate, endDate } = req.query;
    const result = await this.temperatureService.getReport(roomId, startDate, endDate);
    const pdf = await ReportPdfService.generate(result);
    
    res.status(200).json({
      message: 'Relatório gerado com sucesso',
      link: pdf.publicUrl
    });
  }

  getTemperatureReadingsByInterval = async (req, res) => {
          /*
          #swagger.tags = ['Temperature']
          #swagger.summary = 'Obtém todas as leituras de temperatura por intervalo'
          #swagger.description = 'Esse endpoint retorna todas as leituras de temperatura registradas com base no intervalo de datas fornecidos.'
          #swagger.security = [{
            "bearerAuth": []
          }] 
          */

    const { startDate, endDate } = req.query;
    const temperatures = await this.temperatureService.getTemperatureReadingsByInterval(startDate, endDate);
    res.status(200).json(temperatures);
  }

}

export default TemperatureController;