import User from "../models/User.js";
import AuthService from "../services/auth/AuthServices.js";

class AuthController {
    constructor() {
        this.authService = new AuthService(User);
    }
    signIn = async (req, res) => {
        const { email, password } = req.body;
        const result = await this.authService.signIn(email, password);
        res.status(200).json(result);
    }
}

export default AuthController;