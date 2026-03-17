import { jest } from '@jest/globals';
import supertest from 'supertest';
import express from 'express';
import { errorHandler } from '../../middlewares/errorHandler.js';
import AuthController from '../../controllers/AuthControllers.js';

describe('Auth E2E', () => {
  let app;
  let request;
  let authController;

  beforeEach(() => {
    authController = new AuthController();
    app = express();
    app.use(express.json());
    app.post('/api/login', authController.signIn);
    app.use((error, req, res, next) => errorHandler(error, req, res, next));
    request = supertest(app);
  });

  describe('POST /api/login', () => {
    it('should return 200 and token for valid credentials', async () => {
      const mockResult = {
        user: { _id: 'id1', name: 'Admin', email: 'admin@test.com', role: 'admin' },
        token: 'mock-jwt-token'
      };
      authController.authService.signIn = jest.fn().mockResolvedValue(mockResult);

      const res = await request
        .post('/api/login')
        .send({ email: 'admin@test.com', password: 'Password1!' });

      expect(res.status).toBe(200);
      expect(res.body).toHaveProperty('token');
      expect(res.body).toHaveProperty('user');
    });

    it('should return 400 for invalid credentials', async () => {
      authController.authService.signIn = jest.fn().mockRejectedValue(
        Object.assign(new Error('Credenciais inválidas'), { statusCode: 400 })
      );

      const res = await request
        .post('/api/login')
        .send({ email: 'wrong@test.com', password: 'wrong' });

      expect(res.status).toBe(400);
      expect(res.body).toHaveProperty('erro');
    });

    it('should return 400 when email is missing', async () => {
      authController.authService.signIn = jest.fn().mockRejectedValue(
        Object.assign(new Error('Credenciais inválidas'), { statusCode: 400 })
      );

      const res = await request
        .post('/api/login')
        .send({ password: 'Password1!' });

      expect(res.status).toBe(400);
    });
  });
});
