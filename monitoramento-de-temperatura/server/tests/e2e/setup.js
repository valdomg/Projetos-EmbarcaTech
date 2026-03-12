import express from 'express';
import cors from 'cors';
import jsonwebtoken from 'jsonwebtoken';
import { errorHandler } from '../../middlewares/errorHandler.js';

const JWT_SECRET = process.env.JWT_SECRET || 'test-secret-key';

export function createTestApp(router) {
  const app = express();
  app.use(cors());
  app.use(express.json());
  app.use('/api', router);
  app.use((error, req, res, next) => errorHandler(error, req, res, next));
  return app;
}

export function generateToken(payload, secret = JWT_SECRET) {
  return jsonwebtoken.sign(payload, secret, { expiresIn: '1h' });
}

export const adminUser = { id: 'admin123', email: 'admin@test.com', role: 'admin' };
export const regularUser = { id: 'user123', email: 'user@test.com', role: 'user' };

export function getAdminToken() {
  return generateToken(adminUser);
}

export function getUserToken() {
  return generateToken(regularUser);
}
