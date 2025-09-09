import User from "../models/User.js";
import AuthService from "../services/auth/AuthServices.js";

class AuthController {
    constructor() {
        this.authService = new AuthService(User);
    }
    signIn = async (req, res) => {
        try {
            const { email, password } = req.body;
            const result = await this.authService.signIn(email, password);
            res.status(200).json(result);
        } catch (error) {
            console.error("Erro durante o login:", error);
            res.status(401).json({ error: error.message });
        }
    }
}

export default AuthController;