import AuthService from './AuthServices';
import { describe, jest } from '@jest/globals';
import * as bcrypt from 'bcrypt';
describe("AuthService", () => {
    let authService;
    let userModelMock;

    beforeEach(() => {
        userModelMock = {
            findOne: jest.fn(),
        };
        authService = new AuthService(userModelMock);
    });

    describe("signIn", () => {
        it("should throw error if user not found", async () => {
            userModelMock.findOne.mockResolvedValue(null);
            await expect(authService.signIn("test@test.com", "Password1!"))
                .rejects.toThrow("Credenciais inválidas");
        });

        it("should throw error if password is invalid", async () => {
            userModelMock.findOne.mockResolvedValue({ email: "test@test.com", password: "hashedPassword" });
            await expect(authService.signIn("test@test.com", "WrongPassword"))
                .rejects.toThrow("Credenciais inválidas");
        });

        it("should return user and token if credentials are valid", async () => {
            const hashedPassword = await bcrypt.hash("Password1!", 10);

            userModelMock.findOne.mockResolvedValue({ _id: "userId", email: "test@test.com", password: hashedPassword, role: "user" });
            const result = await authService.signIn("test@test.com", "Password1!");
            expect(result).toHaveProperty("user");
            expect(result).toHaveProperty("token");
        });
    });
});