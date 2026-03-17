import { jest } from '@jest/globals';
import jsonwebtoken from 'jsonwebtoken';
import { authenticate } from './auth.js';

describe('authenticate middleware', () => {
  const JWT_SECRET = process.env.JWT_SECRET || 'test-secret';
  let req, res, next;

  beforeEach(() => {
    req = { headers: {} };
    res = {};
    next = jest.fn();
    process.env.JWT_SECRET = JWT_SECRET;
  });

  it('should throw unauthorized error if no token is provided', () => {
    expect(() => authenticate(req, res, next)).toThrow('Token de acesso não fornecido');
  });

  it('should throw unauthorized error if Authorization header has no Bearer token', () => {
    req.headers['authorization'] = '';
    expect(() => authenticate(req, res, next)).toThrow('Token de acesso não fornecido');
  });

  it('should throw forbidden error if token is invalid', () => {
    req.headers['authorization'] = 'Bearer invalidtoken123';
    expect(() => authenticate(req, res, next)).toThrow('Token de acesso inválido');
  });

  it('should call next() and set req.user for valid token', () => {
    const payload = { id: 'user123', email: 'test@test.com', role: 'admin' };
    const token = jsonwebtoken.sign(payload, JWT_SECRET, { expiresIn: '1h' });
    req.headers['authorization'] = `Bearer ${token}`;

    authenticate(req, res, next);

    expect(next).toHaveBeenCalled();
    expect(req.user).toBeDefined();
    expect(req.user.id).toBe('user123');
    expect(req.user.email).toBe('test@test.com');
    expect(req.user.role).toBe('admin');
  });
});
