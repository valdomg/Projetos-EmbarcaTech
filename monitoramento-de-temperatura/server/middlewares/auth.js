import jsonwebtoken from 'jsonwebtoken';
import dotenv from 'dotenv';
import ApiError from '../utils/errors.js';
dotenv.config();

export const authenticate = (req, res, next) => {
    const authHeader = req.headers['authorization'];
    const token = authHeader && authHeader.split(' ')[1];

    if(!token) {
        throw ApiError.unauthorized("Token de acesso não fornecido");
    }

    try {
        const payload = jsonwebtoken.verify(token, process.env.JWT_SECRET);
        req.user = payload;
        return next();
    } catch (error) {
        throw ApiError.forbidden("Token de acesso inválido");
    }

};
