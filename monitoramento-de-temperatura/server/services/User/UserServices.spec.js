import * as bcrypt from 'bcrypt';
import { jest } from '@jest/globals';
import UserService from './UserServices';

describe('UserService', () => {
    let userModelMock;
    let userService;

    beforeEach(() => {
        userModelMock = {
            findOne: jest.fn(),
            save: jest.fn(),
        };
        
        class UserModel {
            constructor(data) {
                Object.assign(this, data);
                this.save = jest.fn().mockResolvedValue(this);
            }
        }
        userService = new UserService(UserModel);
        userService.userModel.findOne = userModelMock.findOne;
    });

    describe('createUser', () => {
        it('should throw error if name or email is missing', async () => {
            await expect(userService.createUser({ email: 'test@test.com', password: 'Password1!' }))
                .rejects.toThrow('Nome e email são obrigatórios');
            await expect(userService.createUser({ name: 'Test', password: 'Password1!' }))
                .rejects.toThrow('Nome e email são obrigatórios');
        });

        it('should throw error if email is already in use', async () => {
            userModelMock.findOne.mockResolvedValue({ email: 'test@test.com' });
            await expect(userService.createUser({ name: 'Test', email: 'test@test.com', password: 'Password1!' }))
                .rejects.toThrow('Email já está em uso');
        });

        it('should throw error if password is invalid', async () => {
            userModelMock.findOne.mockResolvedValue(null);
            await expect(userService.createUser({ name: 'Test', email: 'test@test.com', password: 'short' }))
                .rejects.toThrow('Senha deve ter pelo menos 8 caracteres');
        });

        it('should hash password and save user if data is valid', async () => {
            userModelMock.findOne.mockResolvedValue(null);
            const userData = { name: 'Test', email: 'test@test.com', password: 'Password1!' };
            const user = await userService.createUser(userData);
            expect(user.name).toBe('Test');
            expect(user.email).toBe('test@test.com');
            expect(user.password).not.toBe('Password1!');
            expect(typeof user.password).toBe('string');
        });
    });

    describe('validatePassword', () => {
        it('should return true for valid password', async () => {
            expect(await userService.validatePassword('Password1!')).toBe(true);
        });

        it('should return false for password without uppercase', async () => {
            expect(await userService.validatePassword('password1!')).toBe(false);
        });

        it('should return false for password without lowercase', async () => {
            expect(await userService.validatePassword('PASSWORD1!')).toBe(false);
        });

        it('should return false for password without number', async () => {
            expect(await userService.validatePassword('Password!')).toBe(false);
        });

        it('should return false for password without special character', async () => {
            expect(await userService.validatePassword('Password1')).toBe(false);
        });

        it('should return false for password with less than 8 characters', async () => {
            expect(await userService.validatePassword('P1!a')).toBe(false);
        });
    });

    describe('hashPassword', () => {
        it('should return a hashed password', async () => {
            const plain = 'Password1!';
            const hash = await userService.hashPassword(plain);
            const isMatch = await bcrypt.compare(plain, hash);
            expect(hash).not.toBe(plain);
            expect(isMatch).toBe(true);
        });
    });
});