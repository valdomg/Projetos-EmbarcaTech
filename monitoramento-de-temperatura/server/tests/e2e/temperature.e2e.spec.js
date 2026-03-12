import { jest } from '@jest/globals';
import supertest from 'supertest';
import express from 'express';
import { authenticate } from '../../middlewares/auth.js';
import { errorHandler } from '../../middlewares/errorHandler.js';
import TemperatureController from '../../controllers/TemperatureControllers.js';
import { getAdminToken } from './setup.js';

describe('Temperature E2E', () => {
  let app;
  let request;
  let tempController;
  let adminToken;

  beforeEach(() => {
    tempController = new TemperatureController();
    app = express();
    app.use(express.json());

    app.get('/api/temperatures', authenticate, tempController.getAllTemperatures);
    app.get('/api/temperature/:id', authenticate, tempController.getTemperatureById);
    app.get('/api/temperatures/interval', authenticate, tempController.getTemperatureReadingsByInterval);
    app.get('/api/room/:roomId/temperatures', authenticate, tempController.getRoomTemperatures);
    app.get('/api/room/:roomId/temperatures/interval', authenticate, tempController.getRoomTemperaturesByInterval);
    app.get('/api/room/current', authenticate, tempController.getCurrentTemperature);
    app.get('/api/room/:roomId/report', authenticate, tempController.getReport);
    app.use((error, req, res, next) => errorHandler(error, req, res, next));

    request = supertest(app);
    adminToken = getAdminToken();
  });

  describe('GET /api/temperatures', () => {
    it('should return 200 with all temperature readings', async () => {
      const mockData = [
        { _id: 't1', temperature: 22, humidity: 55, room: { name: 'Sala 1' } },
        { _id: 't2', temperature: 25, humidity: 60, room: { name: 'Sala 2' } }
      ];
      tempController.temperatureService.getTemperatureReadings = jest.fn().mockResolvedValue(mockData);

      const res = await request
        .get('/api/temperatures')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(200);
      expect(res.body).toHaveLength(2);
    });

    it('should return 401 without token', async () => {
      const res = await request.get('/api/temperatures');
      expect(res.status).toBe(401);
    });
  });

  describe('GET /api/temperature/:id', () => {
    it('should return 200 with temperature reading', async () => {
      const mockReading = { _id: 't1', temperature: 22, humidity: 55, room: { name: 'Sala 1' } };
      tempController.temperatureService.getTemperatureReadingById = jest.fn().mockResolvedValue(mockReading);

      const res = await request
        .get('/api/temperature/t1')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(200);
      expect(res.body.temperature).toBe(22);
    });

    it('should return 404 when reading not found', async () => {
      tempController.temperatureService.getTemperatureReadingById = jest.fn().mockRejectedValue(
        Object.assign(new Error('Leitura não encontrada'), { statusCode: 404 })
      );

      const res = await request
        .get('/api/temperature/invalid')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(404);
    });
  });

  describe('GET /api/temperatures/interval', () => {
    it('should return 200 with readings in interval', async () => {
      const mockData = [{ _id: 't1', temperature: 22, humidity: 55 }];
      tempController.temperatureService.getTemperatureReadingsByInterval = jest.fn().mockResolvedValue(mockData);

      const res = await request
        .get('/api/temperatures/interval?startDate=2025-01-01&endDate=2025-01-31')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(200);
      expect(res.body).toHaveLength(1);
    });

    it('should return 400 for invalid dates', async () => {
      tempController.temperatureService.getTemperatureReadingsByInterval = jest.fn().mockRejectedValue(
        Object.assign(new Error('Datas inválidas'), { statusCode: 400 })
      );

      const res = await request
        .get('/api/temperatures/interval?startDate=invalid&endDate=invalid')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(400);
    });
  });

  describe('GET /api/room/:roomId/temperatures', () => {
    it('should return 200 with room temperatures', async () => {
      const mockData = [{ _id: 't1', temperature: 22, humidity: 55, room: { name: 'Sala 1' } }];
      tempController.temperatureService.getRoomTemperatureReadings = jest.fn().mockResolvedValue(mockData);

      const res = await request
        .get('/api/room/roomId1/temperatures')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(200);
    });
  });

  describe('GET /api/room/:roomId/temperatures/interval', () => {
    it('should return 200 with room temperatures in interval', async () => {
      const mockData = [{ _id: 't1', temperature: 22, humidity: 55 }];
      tempController.temperatureService.getRoomTemperatureReadingsByInterval = jest.fn().mockResolvedValue(mockData);

      const res = await request
        .get('/api/room/roomId1/temperatures/interval?startDate=2025-01-01&endDate=2025-01-31')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(200);
    });
  });

  describe('GET /api/room/current', () => {
    it('should return 200 with latest readings per room', async () => {
      const mockData = [
        { _id: 'r1', name: 'Sala 1', temperature: 22, humidity: 55, timestamp: new Date().toISOString() }
      ];
      tempController.temperatureService.getRoomsWithLastReading = jest.fn().mockResolvedValue(mockData);

      const res = await request
        .get('/api/room/current')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(200);
      expect(res.body[0].name).toBe('Sala 1');
    });
  });

  describe('GET /api/room/:roomId/report', () => {
    it('should return 200 with aggregated report data', async () => {
      const mockReport = {
        room: { _id: 'r1', name: 'Sala 1' },
        startDate: '2025-01-01',
        endDate: '2025-01-31',
        readings: [{ _id: { year: 2025, month: 1, day: 15, hour: 14 }, averageTemperature: 22.5 }]
      };
      tempController.temperatureService.getReport = jest.fn().mockResolvedValue(mockReport);

      const res = await request
        .get('/api/room/roomId1/report?startDate=2025-01-01&endDate=2025-01-31')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(200);
      expect(res.body).toHaveProperty('room');
      expect(res.body).toHaveProperty('readings');
    });

    it('should return 404 when room not found', async () => {
      tempController.temperatureService.getReport = jest.fn().mockRejectedValue(
        Object.assign(new Error('Sala não encontrada'), { statusCode: 404 })
      );

      const res = await request
        .get('/api/room/invalid/report?startDate=2025-01-01&endDate=2025-01-31')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(404);
    });
  });
});
