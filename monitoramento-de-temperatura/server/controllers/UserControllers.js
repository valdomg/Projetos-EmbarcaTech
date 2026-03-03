import User from "../models/User.js";
import UserService from "../services/User/UserServices.js";

class UserController {
    constructor() {
        this.userService = new UserService(User);
    }

    createUser = async (req, res) => {
        /*
          #swagger.tags = ['Users']
          #swagger.summary = 'cria um novo usuário'
          #swagger.description = 'Esse endpoint cria um novo usuário no sistema. É necessário fornecer um nome, email e senha válidos no corpo da requisição. O email deve ser único e a senha deve atender aos critérios de segurança estabelecidos. Ao criar um usuário com sucesso, o sistema retorna os detalhes do usuário criado, excluindo a senha por razões de segurança.'
          #swagger.security = [{
            "bearerAuth": []
          }] 
        
          */
        const userData = req.body;
        /*  #swagger.requestBody = {
                    required: true,
                    content: {
                        "application/json": {
                            schema: {
                                type: "object",
                                properties: {
                                    name: { type: "string", example: "João Silva", required: true },
                                    email: { type: "string", example: "joao.silva@example.com", required: true },
                                    password: { type: "string", example: "SenhaSegura123", required: true }
                                }
                            }
                        }
                    }
                }
        */

        const { role } = req.user;
        if (role !== 'admin') {
            return res.status(403).json({ message: 'Acesso negado. Apenas administradores podem criar usuários.' });
        }


        const user = await this.userService.createUser(userData);
        res.status(201).json(user);
        /*  #swagger.responses[201] = {
                    description: "Usuário criado com sucesso",
                    content: {
                        "application/json": {
                            schema:{
                                type: "object",
                                properties: {
                                    _id: { type: "string", example: "64a7b2f5e4b0c8a1d2f3g4h5" },
                                    name: { type: "string", example: "João Silva" },
                                    email: { type: "string", example: "joao.silva@example.com" }
                                }
                            }
                        }           
                    }
                }   
            */
    }

    getUserByEmail = async (req, res) => {
        /*
          #swagger.tags = ['Users']
          #swagger.summary = 'Busca um usuário pelo email'
          #swagger.description = 'Esse endpoint busca um usuário existente no sistema pelo seu email. É necessário fornecer um email válido como parâmetro. Se o usuário for encontrado, o sistema retorna os detalhes do usuário, excluindo a senha por razões de segurança.'
          #swagger.security = [{
            "bearerAuth": []
          }]         
          */
        const { email } = req.params;
        const user = await this.userService.getUserByEmail(email);
        res.status(200).json(user);
        /*  #swagger.responses[200] = {
                    description: "Usuário encontrado com sucesso",
                    content: {
                        "application/json": {
                            schema:{
                                type: "object",
                                properties: {
                                    _id: { type: "string", example: "64a7b2f5e4b0c8a1d2f3g4h5" },
                                    name: { type: "string", example: "João Silva" },
                                    email: { type: "string", example: "joao.silva@example.com" }
                                }
                            }
                        }           
                    }
                }
               #swagger.responses[400] = {
                    description: "Usuário não encontrado",
                    content: {
                        "application/json": {
                            schema:{
                                type: "object",
                                properties: {
                                    error: { type: "string", example: "Usuário não encontrado" }
                                }
                            }
                        }
                    }
                }      
            */
    };

    getAllUsers = async (req, res) => {
        /*
          #swagger.tags = ['Users']
          #swagger.summary = 'Busca todos os usuários'
          #swagger.description = 'Esse endpoint retorna uma lista de todos os usuários cadastrados no sistema.'
          #swagger.security = [{
            "bearerAuth": []
          }] 
          */
        const users = await this.userService.getAllUsers();
        res.status(200).json(users);
        /*  #swagger.responses[200] = {
                    description: "Lista de usuários retornada com sucesso",
                    content: {
                        "application/json": {
                            schema:{
                                type: "array",
                                items: {
                                    type: "object",
                                    properties: {
                                        _id: { type: "string", example: "64a7b2f5e4b0c8a1d2f3g4h5" },
                                        name: { type: "string", example: "João Silva" },
                                        email: { type: "string", example: "joao.silva@example.com" }
                                    }
                                }
                            }
                        }
                    }
                }
            */
    }

    updateUser = async (req, res) => {
        /*
          #swagger.tags = ['Users']
          #swagger.summary = 'Atualiza um usuário existente'
          #swagger.description = 'Esse endpoint atualiza os dados de um usuário existente no sistema. É necessário fornecer um ID de usuário válido como parâmetro e os novos dados no corpo da requisição.'
              #swagger.security = [{
                "bearerAuth": []
              }] 
          */
        const { userId } = req.params;
        /*  #swagger.requestBody = {
                    required: true,
                    content: {
                        "application/json": {
                            schema: {
                                type: "object",
                                properties: {
                                    name: { type: "string", example: "João Silva" },
                                    email: { type: "string", example: "joao.silva@example.com" },
                                    role: { type: "string", example: "admin" }
                                }
                            }
                        }
                    }
                }
        */

        const updateData = req.body;
        const updatedUser = await this.userService.updateUser(userId, updateData);
        res.status(200).json(updatedUser);
    }

    deleteUser = async (req, res) => {
        /*
          #swagger.tags = ['Users']
          #swagger.summary = 'Deleta um usuário existente'
          #swagger.description = 'Esse endpoint deleta um usuário existente no sistema. É necessário fornecer um ID de usuário válido como parâmetro.'
              #swagger.security = [{
                "bearerAuth": []
              }] 
          */

        const { role } = req.user;
        if (role !== 'admin') {
            return res.status(403).json({ message: 'Acesso negado. Apenas administradores podem deletar usuários.' });
        }
        const { userId } = req.params;
        await this.userService.deleteUser(userId);
        res.status(204).send();
    }

    changePassword = async (req, res) => {
        /*
            #swagger.tags = ['Users']
            #swagger.summary = 'Altera a senha de um usuário'
            #swagger.description = 'Esse endpoint permite que um usuário altere sua senha. É necessário fornecer um ID de usuário válido como parâmetro, a senha atual e a nova senha no corpo da requisição.'
                #swagger.security = [{
                    "bearerAuth": []
                }] 
        */
        const { userId } = req.params;
        /*  #swagger.requestBody = {
                    required: true,
                    content: {
                        "application/json": {
                            schema: {
                                type: "object",
                                properties: {
                                    currentPassword: { type: "string", example: "SenhaSegura123", required: true },
                                    newPassword: { type: "string", example: "NovaSenhaSegura123", required: true }
                                }
                            }
                        }
                    }
                }
        */

        const { currentPassword, newPassword } = req.body;
        await this.userService.changePassword(userId, currentPassword, newPassword);
        res.status(200).json({ message: "Senha alterada com sucesso" });
    }
}

export default UserController;