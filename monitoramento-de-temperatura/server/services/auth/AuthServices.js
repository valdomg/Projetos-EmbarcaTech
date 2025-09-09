import jsonwebtoken from 'jsonwebtoken';
import * as bcrypt from 'bcrypt';
import dotenv from 'dotenv';
import ApiError from '../../utils/errors.js';
dotenv.config();

class AuthService {
    constructor(userModel) {
        this.userModel = userModel;
    }

    signIn = async (email, password) => {
        const user = await this.userModel.findOne({ email });
        if (!user) {
            throw ApiError.badRequest("Credenciais inválidas");
        }
        const isMatch = await bcrypt.compare(password, user.password);
        if (!isMatch) {
            throw ApiError.badRequest("Credenciais inválidas");
        }

        const token = jsonwebtoken.sign(
            { id: user._id, email: user.email, role: user.role },
            process.env.JWT_SECRET,
            { expiresIn: '1h' }
        )

        return { user, token };
    }
}

export default AuthService;