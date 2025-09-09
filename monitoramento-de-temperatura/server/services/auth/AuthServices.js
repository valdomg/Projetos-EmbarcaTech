import jsonwebtoken from 'jsonwebtoken';
import * as bcrypt from 'bcrypt';
import dotenv from 'dotenv';
dotenv.config();

class AuthService {
    constructor(userModel) {
        this.userModel = userModel;
    }

    signIn = async (email, password) => {
        const user = await this.userModel.findOne({ email });
        if (!user) {
            throw new Error("User not found");
        }
        const isMatch = await bcrypt.compare(password, user.password);
        if (!isMatch) {
            throw new Error("Invalid password");
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