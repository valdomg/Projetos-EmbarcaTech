import TemperatureService from '../services/TemperatureReading/TemperatureServices.js';
import TemperatureModel from '../models/Temperature.js';
import { ReportPdfService } from '../services/pdf/generatePdf.js';
import ReportModel from '../models/Report.js';

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
          #swagger.responses[200] = {
            description: "Lista de leituras de temperatura",
            content: {
              "application/json": {
                schema: {
                  type: "array",
                  items: { $ref: "#/components/schemas/temperature" }
                }
              }
            }
          }
          #swagger.responses[401] = { description: "Token de acesso não fornecido" }
          #swagger.responses[403] = { description: "Token de acesso inválido" }
          #swagger.responses[500] = { description: "Erro interno do servidor" }
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
          #swagger.parameters['id'] = {
            in: 'path',
            required: true,
            type: 'string',
            description: 'ID da leitura de temperatura'
          }
          #swagger.responses[200] = {
            description: "Leitura de temperatura encontrada",
            content: {
              "application/json": {
                schema: { $ref: "#/components/schemas/temperature" }
              }
            }
          }
          #swagger.responses[400] = { description: "ID inválido" }
          #swagger.responses[401] = { description: "Token de acesso não fornecido" }
          #swagger.responses[403] = { description: "Token de acesso inválido" }
          #swagger.responses[404] = { description: "Leitura não encontrada" }
          #swagger.responses[500] = { description: "Erro interno do servidor" }
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
          #swagger.parameters['roomId'] = {
            in: 'path',
            required: true,
            type: 'string',
            description: 'ID do ambiente'
          }
          #swagger.responses[200] = {
            description: "Lista de leituras do ambiente",
            content: {
              "application/json": {
                schema: {
                  type: "array",
                  items: { $ref: "#/components/schemas/temperature" }
                }
              }
            }
          }
          #swagger.responses[400] = { description: "ID da sala é obrigatório" }
          #swagger.responses[401] = { description: "Token de acesso não fornecido" }
          #swagger.responses[403] = { description: "Token de acesso inválido" }
          #swagger.responses[500] = { description: "Erro interno do servidor" }
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
          #swagger.parameters['roomId'] = {
            in: 'path',
            required: true,
            type: 'string',
            description: 'ID do ambiente'
          }
          #swagger.parameters['startDate'] = {
            in: 'query',
            required: true,
            type: 'string',
            description: 'Data inicial no formato YYYY-MM-DD',
            example: '2025-01-01'
          }
          #swagger.parameters['endDate'] = {
            in: 'query',
            required: true,
            type: 'string',
            description: 'Data final no formato YYYY-MM-DD',
            example: '2025-01-31'
          }
          #swagger.responses[200] = {
            description: "Lista de leituras do ambiente no intervalo",
            content: {
              "application/json": {
                schema: {
                  type: "array",
                  items: { $ref: "#/components/schemas/temperature" }
                }
              }
            }
          }
          #swagger.responses[400] = { description: "Datas inválidas ou intervalo inválido" }
          #swagger.responses[401] = { description: "Token de acesso não fornecido" }
          #swagger.responses[403] = { description: "Token de acesso inválido" }
          #swagger.responses[500] = { description: "Erro interno do servidor" }
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
          #swagger.responses[200] = {
            description: "Lista de ambientes com a última leitura",
            content: {
              "application/json": {
                schema: {
                  type: "array",
                  items: {
                    type: "object",
                    properties: {
                      _id: { type: "string", example: "64a7b2f5e4b0c8a1d2f3g4h5" },
                      name: { type: "string", example: "Sala do Servidor" },
                      temperature: { type: "number", example: 22.5 },
                      humidity: { type: "number", example: 60 },
                      timestamp: { type: "string", example: "2025-01-15T14:30:00Z" }
                    }
                  }
                }
              }
            }
          }
          #swagger.responses[401] = { description: "Token de acesso não fornecido" }
          #swagger.responses[403] = { description: "Token de acesso inválido" }
          #swagger.responses[500] = { description: "Erro interno do servidor" }
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
          #swagger.parameters['roomId'] = {
            in: 'path',
            required: true,
            type: 'string',
            description: 'ID do ambiente'
          }
          #swagger.parameters['startDate'] = {
            in: 'query',
            required: true,
            type: 'string',
            description: 'Data inicial no formato YYYY-MM-DD',
            example: '2025-01-01'
          }
          #swagger.parameters['endDate'] = {
            in: 'query',
            required: true,
            type: 'string',
            description: 'Data final no formato YYYY-MM-DD',
            example: '2025-01-31'
          }
          #swagger.responses[200] = {
            description: "Relatório com dados agregados por hora",
            content: {
              "application/json": {
                schema: {
                  type: "object",
                  properties: {
                    room: { $ref: "#/components/schemas/room" },
                    startDate: { type: "string", example: "2025-01-01" },
                    endDate: { type: "string", example: "2025-01-31" },
                    readings: {
                      type: "array",
                      items: {
                        type: "object",
                        properties: {
                          _id: {
                            type: "object",
                            properties: {
                              year: { type: "number", example: 2025 },
                              month: { type: "number", example: 1 },
                              day: { type: "number", example: 15 },
                              hour: { type: "number", example: 14 }
                            }
                          },
                          averageTemperature: { type: "number", example: 22.5 },
                          minTemperature: { type: "number", example: 20.1 },
                          maxTemperature: { type: "number", example: 24.8 },
                          averageHumidity: { type: "number", example: 55.3 },
                          maxHumidity: { type: "number", example: 62.0 },
                          minHumidity: { type: "number", example: 48.0 }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          #swagger.responses[400] = { description: "Datas inválidas ou intervalo inválido" }
          #swagger.responses[401] = { description: "Token de acesso não fornecido" }
          #swagger.responses[403] = { description: "Token de acesso inválido" }
          #swagger.responses[404] = { description: "Sala não encontrada" }
          #swagger.responses[500] = { description: "Erro interno do servidor" }
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
          #swagger.description = 'Esse endpoint gera um relatório em PDF contendo media, minima e maxima de temperatura e humidade por hora para um ambiente específico com base no ID do ambiente e no intervalo de datas fornecidos. O PDF é gerado utilizando o serviço ReportPdfService e retorna um link para download do arquivo. O relatório também é salvo no histórico do banco de dados.'
          #swagger.security = [{
            "bearerAuth": []
          }]
          #swagger.parameters['roomId'] = {
            in: 'path',
            required: true,
            type: 'string',
            description: 'ID do ambiente'
          }
          #swagger.parameters['startDate'] = {
            in: 'query',
            required: true,
            type: 'string',
            description: 'Data inicial no formato YYYY-MM-DD',
            example: '2025-01-01'
          }
          #swagger.parameters['endDate'] = {
            in: 'query',
            required: true,
            type: 'string',
            description: 'Data final no formato YYYY-MM-DD',
            example: '2025-01-31'
          }
          #swagger.responses[200] = {
            description: "Relatório PDF gerado com sucesso",
            content: {
              "application/json": {
                schema: {
                  type: "object",
                  properties: {
                    message: { type: "string", example: "Relatório gerado com sucesso" },
                    reportId: { type: "string", example: "64a7b2f5e4b0c8a1d2f3g4h5" },
                    link: { type: "string", example: "/reports/Relatorio-Sala-2025-01-01-2025-01-31-1710000000000.pdf" }
                  }
                }
              }
            }
          }
          #swagger.responses[400] = { description: "Datas inválidas ou intervalo inválido" }
          #swagger.responses[401] = { description: "Token de acesso não fornecido" }
          #swagger.responses[403] = { description: "Token de acesso inválido" }
          #swagger.responses[404] = { description: "Sala não encontrada" }
          #swagger.responses[500] = { description: "Erro interno do servidor" }
      */

    const { roomId } = req.params;
    const { startDate, endDate } = req.query;
    const result = await this.temperatureService.getReport(roomId, startDate, endDate);
    const pdf = await ReportPdfService.generate(result);

    const report = new ReportModel({
      room: result.room._id,
      filePath: pdf.filePath
    });
    await report.save();

    res.status(200).json({
      message: 'Relatório gerado com sucesso',
      reportId: report._id,
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
          #swagger.parameters['startDate'] = {
            in: 'query',
            required: true,
            type: 'string',
            description: 'Data inicial no formato YYYY-MM-DD',
            example: '2025-01-01'
          }
          #swagger.parameters['endDate'] = {
            in: 'query',
            required: true,
            type: 'string',
            description: 'Data final no formato YYYY-MM-DD',
            example: '2025-01-31'
          }
          #swagger.responses[200] = {
            description: "Lista de leituras no intervalo",
            content: {
              "application/json": {
                schema: {
                  type: "array",
                  items: { $ref: "#/components/schemas/temperature" }
                }
              }
            }
          }
          #swagger.responses[400] = { description: "Datas inválidas ou intervalo inválido" }
          #swagger.responses[401] = { description: "Token de acesso não fornecido" }
          #swagger.responses[403] = { description: "Token de acesso inválido" }
          #swagger.responses[500] = { description: "Erro interno do servidor" }
      */

    const { startDate, endDate } = req.query;
    const temperatures = await this.temperatureService.getTemperatureReadingsByInterval(startDate, endDate);
    res.status(200).json(temperatures);
  }

}

export default TemperatureController;
