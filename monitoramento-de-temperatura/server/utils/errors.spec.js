import ApiError from "./errors.js";

describe('ApiError', () => {
  it('should create error with default statusCode 400', () => {
    const error = new ApiError('Erro genérico');
    expect(error.message).toBe('Erro genérico');
    expect(error.statusCode).toBe(400);
    expect(error).toBeInstanceOf(Error);
  });

  it('should create error with custom statusCode', () => {
    const error = new ApiError('Erro customizado', 422);
    expect(error.statusCode).toBe(422);
  });

  it('badRequest should return error with statusCode 400', () => {
    const error = ApiError.badRequest('Campo inválido');
    expect(error.message).toBe('Campo inválido');
    expect(error.statusCode).toBe(400);
    expect(error).toBeInstanceOf(ApiError);
  });

  it('notFound should return error with statusCode 404', () => {
    const error = ApiError.notFound('Recurso não encontrado');
    expect(error.message).toBe('Recurso não encontrado');
    expect(error.statusCode).toBe(404);
  });

  it('unauthorized should return error with statusCode 401', () => {
    const error = ApiError.unauthorized('Não autorizado');
    expect(error.message).toBe('Não autorizado');
    expect(error.statusCode).toBe(401);
  });

  it('forbidden should return error with statusCode 403', () => {
    const error = ApiError.forbidden('Acesso negado');
    expect(error.message).toBe('Acesso negado');
    expect(error.statusCode).toBe(403);
  });
});
