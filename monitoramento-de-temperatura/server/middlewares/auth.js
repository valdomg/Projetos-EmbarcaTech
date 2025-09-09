import jsonwebtoken from 'jsonwebtoken';
import dotenv from 'dotenv';
dotenv.config();

export const authenticate = (req, res, next) => {
    const authHeader = req.headers['authorization'];
    const token = authHeader && authHeader.split(' ')[1];
    if (!token) return res.status(401).send("Credenciais não fornecidas");

    try {
        const payload = jsonwebtoken.verify(token, process.env.JWT_SECRET);
        req.user = payload;
        return next();
    } catch (error) {
        console.error("Erro de verificação do token:", error);
        return res.status(403).send("Token inválido");
    }
};
