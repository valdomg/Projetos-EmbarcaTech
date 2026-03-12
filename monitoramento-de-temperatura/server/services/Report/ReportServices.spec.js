import ReportServices from "./ReportServices.js";
import { jest } from '@jest/globals';
import fs from 'fs';

const mockModel = {
  find: jest.fn(),
  findById: jest.fn(),
  findByIdAndDelete: jest.fn(),
};

describe('ReportServices', () => {
  let service;

  beforeEach(() => {
    mockModel.find = jest.fn();
    mockModel.findById = jest.fn();
    mockModel.findByIdAndDelete = jest.fn();

    const model = function (data) {
      return { ...data };
    };

    model.find = mockModel.find;
    model.findById = mockModel.findById;
    model.findByIdAndDelete = mockModel.findByIdAndDelete;

    service = new ReportServices(model);

    jest.clearAllMocks();
  });

  describe('getAllReports', () => {
    it('should return all reports', async () => {
      const mockReports = [
        { _id: 'id1', room: { name: 'Sala 1' }, generatedAt: new Date(), filePath: '/path/report1.pdf' },
        { _id: 'id2', room: { name: 'Sala 2' }, generatedAt: new Date(), filePath: '/path/report2.pdf' },
      ];

      mockModel.find.mockReturnValueOnce({
        populate: jest.fn().mockResolvedValue(mockReports)
      });

      const result = await service.getAllReports();

      expect(mockModel.find).toHaveBeenCalled();
      expect(result).toEqual(mockReports);
    });

    it('should return empty array if no reports found', async () => {
      mockModel.find.mockReturnValueOnce({
        populate: jest.fn().mockResolvedValue([])
      });

      const result = await service.getAllReports();

      expect(mockModel.find).toHaveBeenCalled();
      expect(result).toEqual([]);
    });
  });

  describe('getReportById', () => {
    it('should return report by id', async () => {
      const mockReport = { _id: 'id123', room: { name: 'Sala 1' }, generatedAt: new Date(), filePath: '/path/report.pdf' };

      mockModel.findById.mockReturnValueOnce({
        populate: jest.fn().mockResolvedValue(mockReport)
      });

      const result = await service.getReportById('id123');

      expect(mockModel.findById).toHaveBeenCalledWith('id123');
      expect(result).toEqual(mockReport);
    });

    it('should throw error if reportId is missing', async () => {
      await expect(service.getReportById()).rejects.toThrow('reportId é obrigatório');
    });

    it('should throw error if report not found', async () => {
      mockModel.findById.mockReturnValueOnce({
        populate: jest.fn().mockResolvedValue(null)
      });

      await expect(service.getReportById('id123')).rejects.toThrow('Relatório não encontrado');
    });
  });

  describe('deleteReport', () => {
    it('should delete report and physical file correctly', async () => {
      const mockReport = { _id: 'id123', filePath: '/path/report.pdf' };

      mockModel.findById.mockResolvedValue(mockReport);
      mockModel.findByIdAndDelete.mockResolvedValue(mockReport);

      const existsSpy = jest.spyOn(fs, 'existsSync').mockReturnValue(true);
      const unlinkSpy = jest.spyOn(fs, 'unlinkSync').mockReturnValue(undefined);

      await service.deleteReport('id123');

      expect(mockModel.findById).toHaveBeenCalledWith('id123');
      expect(existsSpy).toHaveBeenCalledWith('/path/report.pdf');
      expect(unlinkSpy).toHaveBeenCalledWith('/path/report.pdf');
      expect(mockModel.findByIdAndDelete).toHaveBeenCalledWith('id123');

      existsSpy.mockRestore();
      unlinkSpy.mockRestore();
    });

    it('should throw error if reportId is missing', async () => {
      await expect(service.deleteReport()).rejects.toThrow('reportId é obrigatório');
    });

    it('should throw error if report not found', async () => {
      mockModel.findById.mockResolvedValue(null);

      await expect(service.deleteReport('id123')).rejects.toThrow('Relatório não encontrado');
    });

    it('should delete record even if physical file does not exist', async () => {
      const mockReport = { _id: 'id123', filePath: '/path/report.pdf' };

      mockModel.findById.mockResolvedValue(mockReport);
      mockModel.findByIdAndDelete.mockResolvedValue(mockReport);

      const existsSpy = jest.spyOn(fs, 'existsSync').mockReturnValue(false);
      const unlinkSpy = jest.spyOn(fs, 'unlinkSync');

      await service.deleteReport('id123');

      expect(existsSpy).toHaveBeenCalledWith('/path/report.pdf');
      expect(unlinkSpy).not.toHaveBeenCalled();
      expect(mockModel.findByIdAndDelete).toHaveBeenCalledWith('id123');

      existsSpy.mockRestore();
      unlinkSpy.mockRestore();
    });

    it('should delete record even if file deletion throws error', async () => {
      const mockReport = { _id: 'id123', filePath: '/path/report.pdf' };

      mockModel.findById.mockResolvedValue(mockReport);
      mockModel.findByIdAndDelete.mockResolvedValue(mockReport);

      const existsSpy = jest.spyOn(fs, 'existsSync').mockReturnValue(true);
      const unlinkSpy = jest.spyOn(fs, 'unlinkSync').mockImplementation(() => { throw new Error('Permission denied'); });
      const consoleSpy = jest.spyOn(console, 'error').mockImplementation(() => {});

      await service.deleteReport('id123');

      expect(mockModel.findByIdAndDelete).toHaveBeenCalledWith('id123');

      existsSpy.mockRestore();
      unlinkSpy.mockRestore();
      consoleSpy.mockRestore();
    });
  });

});
