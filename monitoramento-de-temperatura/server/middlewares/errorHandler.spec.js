import { errorHandler } from "./errorHandler.js";

describe('errorHandler', () => {
  let mockRes;

  beforeEach(() => {
    mockRes = {
      status: function (code) {
        this.statusCode = code;
        return this;
      },
      json: function (data) {
        this.body = data;
        return this;
      }
    };
  });

  it('should return statusCode and message from ApiError', () => {
    const error = { statusCode: 400, message: 'Campo inválido' };
    errorHandler(error, null, mockRes);
    expect(mockRes.statusCode).toBe(400);
    expect(mockRes.body).toEqual({ erro: 'Campo inválido' });
  });

  it('should return 500 when statusCode is not provided', () => {
    const error = { message: 'Algo deu errado' };
    errorHandler(error, null, mockRes);
    expect(mockRes.statusCode).toBe(500);
    expect(mockRes.body).toEqual({ erro: 'Algo deu errado' });
  });

  it('should return default message when message is not provided', () => {
    const error = {};
    errorHandler(error, null, mockRes);
    expect(mockRes.statusCode).toBe(500);
    expect(mockRes.body).toEqual({ erro: 'Erro interno do servidor' });
  });
});
