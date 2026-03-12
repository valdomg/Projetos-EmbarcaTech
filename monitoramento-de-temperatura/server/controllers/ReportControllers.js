import ReportServices from '../services/Report/ReportServices.js';
import ReportModel from '../models/Report.js';

class ReportController {
  constructor() {
    this.reportService = new ReportServices(ReportModel);
  }

  getAllReports = async (req, res) => {
    /*
      #swagger.tags = ['Report']
      #swagger.summary = 'Lista todos os relatórios'
      #swagger.description = 'Retorna todos os relatórios gerados no sistema, com o nome do ambiente populado.'
      #swagger.security = [{
        "bearerAuth": []
      }]
      #swagger.responses[200] = {
        description: "Lista de relatórios",
        content: {
          "application/json": {
            schema: {
              type: "array",
              items: { $ref: "#/components/schemas/report" }
            }
          }
        }
      }
      #swagger.responses[401] = { description: "Token de acesso não fornecido" }
      #swagger.responses[403] = { description: "Token de acesso inválido" }
      #swagger.responses[500] = { description: "Erro interno do servidor" }
    */

    const reports = await this.reportService.getAllReports();
    res.status(200).json(reports);
  }

  getReportById = async (req, res) => {
    /*
      #swagger.tags = ['Report']
      #swagger.summary = 'Obtém um relatório pelo ID'
      #swagger.description = 'Retorna um relatório específico com base no ID fornecido, com o nome do ambiente populado.'
      #swagger.security = [{
        "bearerAuth": []
      }]
      #swagger.parameters['reportId'] = {
        in: 'path',
        required: true,
        type: 'string',
        description: 'ID do relatório'
      }
      #swagger.responses[200] = {
        description: "Relatório encontrado",
        content: {
          "application/json": {
            schema: { $ref: "#/components/schemas/report" }
          }
        }
      }
      #swagger.responses[400] = { description: "reportId é obrigatório" }
      #swagger.responses[401] = { description: "Token de acesso não fornecido" }
      #swagger.responses[403] = { description: "Token de acesso inválido" }
      #swagger.responses[404] = { description: "Relatório não encontrado" }
      #swagger.responses[500] = { description: "Erro interno do servidor" }
    */

    const { reportId } = req.params;
    const report = await this.reportService.getReportById(reportId);
    res.status(200).json(report);
  }

  deleteReport = async (req, res) => {
    /*
      #swagger.tags = ['Report']
      #swagger.summary = 'Deleta um relatório pelo ID'
      #swagger.description = 'Remove o registro do relatório do banco de dados e apaga o arquivo PDF correspondente do servidor. Apenas administradores podem realizar esta operação.'
      #swagger.security = [{
        "bearerAuth": []
      }]
      #swagger.parameters['reportId'] = {
        in: 'path',
        required: true,
        type: 'string',
        description: 'ID do relatório'
      }
      #swagger.responses[204] = { description: "Relatório deletado com sucesso" }
      #swagger.responses[400] = { description: "reportId é obrigatório" }
      #swagger.responses[401] = { description: "Token de acesso não fornecido" }
      #swagger.responses[403] = { description: "Acesso negado. Apenas administradores podem deletar relatórios." }
      #swagger.responses[404] = { description: "Relatório não encontrado" }
      #swagger.responses[500] = { description: "Erro interno do servidor" }
    */

    const { role } = req.user;
    if (role !== 'admin') {
      return res.status(403).json({
        message: 'Acesso negado. Apenas administradores podem deletar relatórios.'
      });
    }

    const { reportId } = req.params;
    await this.reportService.deleteReport(reportId);
    res.status(204).send();
  }
}

export default ReportController;
