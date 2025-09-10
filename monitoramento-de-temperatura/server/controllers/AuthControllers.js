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
          #swagger.description = 'Esse endpoint permite que um usuário faça login no sistema.'
        */

        /*  #swagger.requestBody = {
                required: true,
                content: {
                    "application/json": {
                        schema: {
                            type: "object",
                            properties: {
                                email: { type: "string", example: "joao.silva@example.com", required: true },
                                password: { type: "string", example: "SenhaSegura123", required: true }
                            }
                        }
                    }
                }
            }
    */

        const { email, password } = req.body;
        const result = await this.authService.signIn(email, password);
        res.status(200).json(result);
    }
}

export default AuthController;