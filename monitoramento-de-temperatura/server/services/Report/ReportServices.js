import ApiError from "../../utils/errors.js";
import fs from 'fs';

class ReportServices {
  constructor(ReportModel) {
    this.ReportModel = ReportModel;
  }

  getAllReports = async () => {
    return await this.ReportModel.find().populate('room', 'name');
  }

  getReportById = async (reportId) => {
    if (!reportId) {
      throw ApiError.badRequest("reportId é obrigatório");
    }

    const report = await this.ReportModel.findById(reportId).populate('room', 'name');

    if (!report) {
      throw ApiError.notFound("Relatório não encontrado");
    }

    return report;
  }

  deleteReport = async (reportId) => {
    if (!reportId) {
      throw ApiError.badRequest("reportId é obrigatório");
    }

    const report = await this.ReportModel.findById(reportId);

    if (!report) {
      throw ApiError.notFound("Relatório não encontrado");
    }

    try {
      if (fs.existsSync(report.filePath)) {
        fs.unlinkSync(report.filePath);
      }
    } catch (err) {
      console.error('Erro ao deletar arquivo do relatório:', err);
    }

    await this.ReportModel.findByIdAndDelete(reportId);
  }
}

export default ReportServices;
