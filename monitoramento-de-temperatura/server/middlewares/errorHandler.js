export const errorHandler = (err, _, res) => {
  const statusCode = err.statusCode || 500;
  const message = err.message || "Erro interno do servidor";
  res.status(statusCode).json({ erro: message });
};