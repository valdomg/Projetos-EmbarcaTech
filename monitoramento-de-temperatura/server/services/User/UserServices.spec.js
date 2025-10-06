import * as bcrypt from 'bcrypt';
import { describe, jest } from '@jest/globals';
import UserService from './UserServices';

describe('UserService', () => {
    let userModelMock;
    let userService;
    let saveUser;

    beforeEach(() => {
        saveUser = jest.fn().mockResolvedValue('mocked saved reading');
        userModelMock = {
            find: jest.fn(),
            findById: jest.fn(),
            findByIdAndDelete: jest.fn(),
            create: jest.fn(),
            findOne: jest.fn(),
            findByIdAndUpdate: jest.fn(),
        };

        const model = function (data) {
            return {
                ...data,
                save: saveUser
            };
        };


        model.find = userModelMock.find;
        model.findById = userModelMock.findById;
        model.findByIdAndDelete = userModelMock.findByIdAndDelete;
        model.create = userModelMock.create;
        model.findOne = userModelMock.findOne;
        model.findByIdAndUpdate = userModelMock.findByIdAndUpdate;

        userService = new UserService(model);
        jest.clearAllMocks();
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

    describe('getUserByEmail', () => {
        it('should throw error if email is missing', async () => {
            await expect(userService.getUserByEmail()).rejects.toThrow('Email é obrigatório');
        });
        it('should return user if email exists', async () => {
            const user = { name: 'Test', email: 'test@test.com', password: 'Password1!' };
            userModelMock.findOne.mockResolvedValue(user);
            await expect(userService.getUserByEmail('test@test.com')).resolves.toEqual(user);
        });
        it('should return null if email does not exist', async () => {
            userModelMock.findOne.mockResolvedValue(null);
            await expect(userService.getUserByEmail('nonexistent@test.com')).resolves.toBeNull();
        });

    });

    describe('getUserById', () => {
        it('should throw error if userId is not provided', async () => {
            await expect(userService.getUserById()).rejects.toThrow('ID do usuário é obrigatorio');
        });

        it('should return user if found', async () => {
            const user = { _id: 'userId', name: 'Test', email: 'test@test.com' };
            userModelMock.findById.mockResolvedValue(user);
            await expect(userService.getUserById('userId')).resolves.toEqual(user);
        });

        it('should throw error if user not found', async () => {
            userModelMock.findById.mockResolvedValue(null);
            await expect(userService.getUserById('nonexistentId')).rejects.toThrow('Usuário não encontrado');
        });
    });

    describe('getAllUsers', () => {
        it('should return an array of users', async () => {
            const users = [
                { _id: 'userId1', name: 'Test1', email: 'test1@test.com' },
                { _id: 'userId2', name: 'Test2', email: 'test2@test.com' }
            ];
            userModelMock.find.mockResolvedValue(users);
            await expect(userService.getAllUsers()).resolves.toEqual(users);
        });
    });

    describe('updateUser', () => {
        it('should throw error if email is already in use', async () => {
            userModelMock.findOne.mockResolvedValue({ email: 'test@test.com' });
            await expect(userService.updateUser('userId', { email: 'test@test.com' }))
                .rejects.toThrow('Email já está em uso');
        });

        it('should throw error if user not found', async () => {
            userModelMock.findOne.mockResolvedValue(null);
            userModelMock.findByIdAndUpdate.mockResolvedValue(null);
            await expect(userService.updateUser('userId', { name: 'New Name', email: 'new@test.com' }))
                .rejects.toThrow('Usuário não encontrado');
        });

        it('should update user if data is valid', async () => {
            userModelMock.findOne.mockResolvedValue(null);
            userModelMock.findByIdAndUpdate.mockResolvedValue({
                _id: 'userId',
                name: 'Updated',
                email: 'new@test.com'
            });

            const result = await userService.updateUser('userId', { name: 'Updated', email: 'new@test.com' });

            expect(result).toEqual({
                _id: 'userId',
                name: 'Updated',
                email: 'new@test.com'
            });
        });
    });

    describe("deleteUser", () => {
        it("should throw error if userId is not provided", async () => {
            await expect(userService.deleteUser()).rejects.toThrow("ID do usuário é obrigatorio");
        });

        it("should throw error if user is not found", async () => {
            userModelMock.findById.mockResolvedValue(null);
            await expect(userService.deleteUser("nonexistentId")).rejects.toThrow("Usuário não encontrado");
        });

        it("should delete user if userId is valid", async () => {
            userModelMock.findById.mockResolvedValue({ _id: "validId", name: "User" });
            userModelMock.findByIdAndDelete.mockResolvedValue({});
            await expect(userService.deleteUser("validId")).resolves.toBeUndefined();
            expect(userModelMock.findById).toHaveBeenCalledWith("validId");
            expect(userModelMock.findByIdAndDelete).toHaveBeenCalledWith("validId");
        });
    });

});