import { jest } from '@jest/globals';
import supertest from 'supertest';
import express from 'express';
import { authenticate } from '../../middlewares/auth.js';
import { errorHandler } from '../../middlewares/errorHandler.js';
import RoomController from '../../controllers/RoomControllers.js';
import { getAdminToken, getUserToken } from './setup.js';

describe('Room E2E', () => {
  let app;
  let request;
  let roomController;
  let adminToken;
  let userToken;

  beforeEach(() => {
    roomController = new RoomController();
    app = express();
    app.use(express.json());

    app.post('/api/room', authenticate, roomController.createRoom);
    app.get('/api/rooms', authenticate, roomController.getAllRooms);
    app.get('/api/room/id/:roomId', authenticate, roomController.getRoomById);
    app.get('/api/room/microcontroller/:microcontrollerId', authenticate, roomController.getRoomByMicrocontrollerId);
    app.patch('/api/room/:roomId', authenticate, roomController.updateRoom);
    app.delete('/api/room/:roomId', authenticate, roomController.deleteRoom);
    app.use((error, req, res, next) => errorHandler(error, req, res, next));

    request = supertest(app);
    adminToken = getAdminToken();
    userToken = getUserToken();
  });

  describe('POST /api/room', () => {
    it('should return 201 when admin creates a room', async () => {
      const mockRoom = { _id: 'roomId', name: 'Sala 1', microcontrollerId: 'MCU001' };
      roomController.roomService.createRoom = jest.fn().mockResolvedValue(mockRoom);

      const res = await request
        .post('/api/room')
        .set('Authorization', `Bearer ${adminToken}`)
        .send({ name: 'Sala 1', microcontrollerId: 'MCU001' });

      expect(res.status).toBe(201);
      expect(res.body.name).toBe('Sala 1');
    });

    it('should return 403 when non-admin tries to create room', async () => {
      const res = await request
        .post('/api/room')
        .set('Authorization', `Bearer ${userToken}`)
        .send({ name: 'Sala 1', microcontrollerId: 'MCU001' });

      expect(res.status).toBe(403);
    });

    it('should return 401 when no token is provided', async () => {
      const res = await request
        .post('/api/room')
        .send({ name: 'Sala 1', microcontrollerId: 'MCU001' });

      expect(res.status).toBe(401);
    });
  });

  describe('GET /api/rooms', () => {
    it('should return 200 with list of rooms', async () => {
      const mockRooms = [
        { _id: 'r1', name: 'Sala 1', microcontrollerId: 'MCU001' },
        { _id: 'r2', name: 'Sala 2', microcontrollerId: 'MCU002' }
      ];
      roomController.roomService.getAllRooms = jest.fn().mockResolvedValue(mockRooms);

      const res = await request
        .get('/api/rooms')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(200);
      expect(res.body).toHaveLength(2);
    });
  });

  describe('GET /api/room/id/:roomId', () => {
    it('should return 200 with room data', async () => {
      const mockRoom = { _id: 'r1', name: 'Sala 1', microcontrollerId: 'MCU001' };
      roomController.roomService.getRoomById = jest.fn().mockResolvedValue(mockRoom);

      const res = await request
        .get('/api/room/id/r1')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(200);
      expect(res.body.name).toBe('Sala 1');
    });

    it('should return 400 when room not found', async () => {
      roomController.roomService.getRoomById = jest.fn().mockRejectedValue(
        Object.assign(new Error('Sala não encontrada'), { statusCode: 400 })
      );

      const res = await request
        .get('/api/room/id/invalid')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(400);
    });
  });

  describe('GET /api/room/microcontroller/:microcontrollerId', () => {
    it('should return 200 with room data', async () => {
      const mockRoom = { _id: 'r1', name: 'Sala 1', microcontrollerId: 'MCU001' };
      roomController.roomService.getRoomByMicrocontrollerId = jest.fn().mockResolvedValue(mockRoom);

      const res = await request
        .get('/api/room/microcontroller/MCU001')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(200);
      expect(res.body.microcontrollerId).toBe('MCU001');
    });
  });

  describe('PATCH /api/room/:roomId', () => {
    it('should return 200 when admin updates room', async () => {
      const mockUpdated = { _id: 'r1', name: 'Sala Atualizada', microcontrollerId: 'MCU001' };
      roomController.roomService.updateRoom = jest.fn().mockResolvedValue(mockUpdated);

      const res = await request
        .patch('/api/room/r1')
        .set('Authorization', `Bearer ${adminToken}`)
        .send({ name: 'Sala Atualizada' });

      expect(res.status).toBe(200);
      expect(res.body.name).toBe('Sala Atualizada');
    });

    it('should return 403 when non-admin tries to update', async () => {
      const res = await request
        .patch('/api/room/r1')
        .set('Authorization', `Bearer ${userToken}`)
        .send({ name: 'Sala Atualizada' });

      expect(res.status).toBe(403);
    });
  });

  describe('DELETE /api/room/:roomId', () => {
    it('should return 204 when admin deletes room', async () => {
      roomController.roomService.deleteRoom = jest.fn().mockResolvedValue(undefined);

      const res = await request
        .delete('/api/room/r1')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(204);
    });

    it('should return 403 when non-admin tries to delete', async () => {
      const res = await request
        .delete('/api/room/r1')
        .set('Authorization', `Bearer ${userToken}`);

      expect(res.status).toBe(403);
    });
  });
});
