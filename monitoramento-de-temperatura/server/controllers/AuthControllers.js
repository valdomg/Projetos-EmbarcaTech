import User from "../models/User.js";
import AuthService from "../services/auth/AuthServices.js";

class AuthController {
    constructor() {
        this.authService = new AuthService(User);
    }
    signIn = async (req, res) => {
        /*
          #swagger.tags = ['Auth']
          #swagger.summary = 'Realiza o login de um usuário'
          #swagger.description = 'Esse endpoint permite que um usuário faça login no sistema. Retorna um token JWT válido por 1 hora.'
          #swagger.requestBody = {
                required: true,
                content: {
                    "application/json": {
                        schema: {
                            type: "object",
                            properties: {
                                email: { type: "string", example: "joao.silva@example.com" },
                                password: { type: "string", example: "SenhaSegura@123" }
                            },
                            required: ["email", "password"]
                        }
                    }
                }
            }
          #swagger.responses[200] = {
            description: "Login realizado com sucesso",
            content: {
              "application/json": {
                schema: {
                  type: "object",
                  properties: {
                    user: {
                      type: "object",
                      properties: {
                        _id: { type: "string", example: "64a7b2f5e4b0c8a1d2f3g4h5" },
                        name: { type: "string", example: "João Silva" },
                        email: { type: "string", example: "joao.silva@example.com" },
                        role: { type: "string", example: "admin" }
                      }
                    },
                    token: { type: "string", example: "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9..." }
                  }
                }
              }
            }
          }
          #swagger.responses[400] = { description: "Credenciais inválidas" }
          #swagger.responses[500] = { description: "Erro interno do servidor" }
        */

        const { email, password } = req.body;
        const result = await this.authService.signIn(email, password);
        res.status(200).json(result);
    }
}

export default AuthController;