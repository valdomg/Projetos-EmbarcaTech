import { jest } from '@jest/globals';
import supertest from 'supertest';
import express from 'express';
import { authenticate } from '../../middlewares/auth.js';
import { errorHandler } from '../../middlewares/errorHandler.js';
import UserController from '../../controllers/UserControllers.js';
import { getAdminToken, getUserToken } from './setup.js';

describe('User E2E', () => {
  let app;
  let request;
  let userController;
  let adminToken;
  let userToken;

  beforeEach(() => {
    userController = new UserController();
    app = express();
    app.use(express.json());

    app.post('/api/user', authenticate, userController.createUser);
    app.get('/api/users', authenticate, userController.getAllUsers);
    app.get('/api/user/:email', authenticate, userController.getUserByEmail);
    app.patch('/api/user/:userId', authenticate, userController.updateUser);
    app.delete('/api/user/:userId', authenticate, userController.deleteUser);
    app.patch('/api/user/:userId/change-password', authenticate, userController.changePassword);
    app.use((error, req, res, next) => errorHandler(error, req, res, next));

    request = supertest(app);
    adminToken = getAdminToken();
    userToken = getUserToken();
  });

  describe('POST /api/user', () => {
    it('should return 201 when admin creates a user', async () => {
      const mockUser = { id: 'newUser', name: 'João', email: 'joao@test.com', role: 'user' };
      userController.userService.createUser = jest.fn().mockResolvedValue(mockUser);

      const res = await request
        .post('/api/user')
        .set('Authorization', `Bearer ${adminToken}`)
        .send({ name: 'João', email: 'joao@test.com', password: 'SenhaSegura@1' });

      expect(res.status).toBe(201);
      expect(res.body.name).toBe('João');
    });

    it('should return 403 when non-admin tries to create a user', async () => {
      const res = await request
        .post('/api/user')
        .set('Authorization', `Bearer ${userToken}`)
        .send({ name: 'João', email: 'joao@test.com', password: 'SenhaSegura@1' });

      expect(res.status).toBe(403);
    });

    it('should return 401 when no token is provided', async () => {
      const res = await request
        .post('/api/user')
        .send({ name: 'João', email: 'joao@test.com', password: 'SenhaSegura@1' });

      expect(res.status).toBe(401);
    });
  });

  describe('GET /api/users', () => {
    it('should return 200 with list of users', async () => {
      const mockUsers = [
        { _id: 'id1', name: 'User 1', email: 'u1@test.com', role: 'user' },
        { _id: 'id2', name: 'User 2', email: 'u2@test.com', role: 'admin' }
      ];
      userController.userService.getAllUsers = jest.fn().mockResolvedValue(mockUsers);

      const res = await request
        .get('/api/users')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(200);
      expect(res.body).toHaveLength(2);
    });
  });

  describe('GET /api/user/:email', () => {
    it('should return 200 with user data', async () => {
      const mockUser = { _id: 'id1', name: 'User 1', email: 'u1@test.com', role: 'user' };
      userController.userService.getUserByEmail = jest.fn().mockResolvedValue(mockUser);

      const res = await request
        .get('/api/user/u1@test.com')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(200);
      expect(res.body.email).toBe('u1@test.com');
    });
  });

  describe('PATCH /api/user/:userId', () => {
    it('should return 200 when updating user', async () => {
      const mockUpdated = { _id: 'id1', name: 'Updated', email: 'u1@test.com', role: 'user' };
      userController.userService.updateUser = jest.fn().mockResolvedValue(mockUpdated);

      const res = await request
        .patch('/api/user/id1')
        .set('Authorization', `Bearer ${adminToken}`)
        .send({ name: 'Updated' });

      expect(res.status).toBe(200);
      expect(res.body.name).toBe('Updated');
    });
  });

  describe('DELETE /api/user/:userId', () => {
    it('should return 204 when admin deletes user', async () => {
      userController.userService.deleteUser = jest.fn().mockResolvedValue(undefined);

      const res = await request
        .delete('/api/user/id1')
        .set('Authorization', `Bearer ${adminToken}`);

      expect(res.status).toBe(204);
    });

    it('should return 403 when non-admin tries to delete', async () => {
      const res = await request
        .delete('/api/user/id1')
        .set('Authorization', `Bearer ${userToken}`);

      expect(res.status).toBe(403);
    });
  });

  describe('PATCH /api/user/:userId/change-password', () => {
    it('should return 200 when password is changed', async () => {
      userController.userService.changePassword = jest.fn().mockResolvedValue(undefined);

      const res = await request
        .patch('/api/user/id1/change-password')
        .set('Authorization', `Bearer ${adminToken}`)
        .send({ currentPassword: 'OldPass1!', newPassword: 'NewPass1!' });

      expect(res.status).toBe(200);
      expect(res.body.message).toBe('Senha alterada com sucesso');
    });
  });
});
