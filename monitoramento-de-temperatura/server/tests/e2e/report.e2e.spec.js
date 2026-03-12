import { jest } from '@jest/globals';
import supertest from 'supertest';
import express from 'express';
import { authenticate } from '../../middlewares/auth.js';
import { errorHandler } from '../../middlewares/errorHandler.js';
import ReportController from '../../controllers/ReportControllers.js';
import { getAdminToken, getUserToken } from './setup.js';

describe('Report E2E', () => {
  let app;
  let request;
  let reportController;
  let adminToken;
  let userToken;

  beforeEach(() => {
    reportController = new ReportController();
    app = express();
    app.use(express.json());

    app.get('/api/reports', authenticate, reportController.getAllReports);
    app.get('/api/report/:reportId', authenticate, reportController.getReportById);
    app.delete('/api/report/:reportId', authenticate, reportController.deleteReport);
    app.use((error, req, res, next) => errorHandler(error, req, res, next));

    request = supertest(app);
    adminToken = getAdminToken();
    userToken = getUserToken();
  });

  describe('GET /api/reports', () => {
    it('should return 200 with list of reports', async () => {
      const mockReports = [
        { _id: 'rep1', room: { name: 'Sala 1' }, generatedAt: new Date().toISOString(), filePath: '/path/report1.pdf' },
        { _id: 'rep2', room: { name: 'Sala 2' }, generatedAt: new Date().toISOString(), filePath: '/path/report2.pdf' }
      ];
      reportController.reportService.getAllReports = jest.fn().mockResolvedValue(mockReports);

      const res = await request
        .get('/api/reports')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(200);
      expect(res.body).toHaveLength(2);
    });

    it('should return 200 with empty array when no reports exist', async () => {
      reportController.reportService.getAllReports = jest.fn().mockResolvedValue([]);

      const res = await request
        .get('/api/reports')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(200);
      expect(res.body).toEqual([]);
    });

    it('should return 401 when no token is provided', async () => {
      const res = await request.get('/api/reports');

      expect(res.status).toBe(401);
    });
  });

  describe('GET /api/report/:reportId', () => {
    it('should return 200 with report data', async () => {
      const mockReport = { _id: 'rep1', room: { name: 'Sala 1' }, generatedAt: new Date().toISOString(), filePath: '/path/report.pdf' };
      reportController.reportService.getReportById = jest.fn().mockResolvedValue(mockReport);

      const res = await request
        .get('/api/report/rep1')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(200);
      expect(res.body._id).toBe('rep1');
    });

    it('should return 404 when report not found', async () => {
      reportController.reportService.getReportById = jest.fn().mockRejectedValue(
        Object.assign(new Error('Relatório não encontrado'), { statusCode: 404 })
      );

      const res = await request
        .get('/api/report/invalid')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(404);
      expect(res.body.erro).toBe('Relatório não encontrado');
    });
  });

  describe('DELETE /api/report/:reportId', () => {
    it('should return 204 when admin deletes report', async () => {
      reportController.reportService.deleteReport = jest.fn().mockResolvedValue(undefined);

      const res = await request
        .delete('/api/report/rep1')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(204);
    });

    it('should return 403 when non-admin tries to delete', async () => {
      const res = await request
        .delete('/api/report/rep1')
        .set('Authorization', `Bearer ${userToken}`);

      expect(res.status).toBe(403);
    });

    it('should return 404 when report not found', async () => {
      reportController.reportService.deleteReport = jest.fn().mockRejectedValue(
        Object.assign(new Error('Relatório não encontrado'), { statusCode: 404 })
      );

      const res = await request
        .delete('/api/report/rep1')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(404);
    });

    it('should return 401 when no token is provided', async () => {
      const res = await request.delete('/api/report/rep1');

      expect(res.status).toBe(401);
    });
  });
});
