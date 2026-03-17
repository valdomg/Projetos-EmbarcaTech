import RoomService from '../services/Room/RoomServices.js';
import RoomModel from '../models/Room.js';

class RoomController {
  constructor() {
    this.roomService = new RoomService(RoomModel);
  }

  createRoom = async (req, res) => {
    /*
      #swagger.tags = ['Room']
      #swagger.summary = 'Cria um novo ambiente'
      #swagger.description = 'Esse endpoint cria um novo ambiente no sistema. Apenas administradores podem criar ambientes.'
      #swagger.security = [{
        "bearerAuth": []
      }]
      #swagger.requestBody = {
        required: true,
        content: {
          "application/json": {
            schema: {
              type: "object",
              properties: {
                name: { type: "string", example: "Sala do Servidor" },
                microcontrollerId: { type: "string", example: "MCU123456" },
                minTemperature: { type: "number", example: 18 },
                maxTemperature: { type: "number", example: 27 },
                minHumidity: { type: "number", example: 30 },
                maxHumidity: { type: "number", example: 60 }
              },
              required: ["name", "microcontrollerId"]
            }
          }
        }
      }
      #swagger.responses[201] = {
        description: "Ambiente criado com sucesso",
        content: {
          "application/json": {
            schema: { $ref: "#/components/schemas/room" }
          }
        }
      }
      #swagger.responses[400] = { description: "Nome e microcontrollerId são obrigatórios" }
      #swagger.responses[401] = { description: "Token de acesso não fornecido" }
      #swagger.responses[403] = { description: "Acesso negado. Apenas administradores podem criar ambientes." }
      #swagger.responses[500] = { description: "Erro interno do servidor" }
    */

    const { role } = req.user;
    if (role !== 'admin') {
      return res.status(403).json({ message: 'Acesso negado. Apenas administradores podem criar ambientes.' });
    }

    const room = await this.roomService.createRoom(req.body);
    res.status(201).json(room);
  }

  getAllRooms = async (req, res) => {
    /*
      #swagger.tags = ['Room']
      #swagger.summary = 'Obtém todos os ambientes'
      #swagger.description = 'Esse endpoint retorna todos os ambientes registrados no sistema.'
      #swagger.security = [{
        "bearerAuth": []
      }]
      #swagger.responses[200] = {
        description: "Lista de ambientes",
        content: {
          "application/json": {
            schema: {
              type: "array",
              items: { $ref: "#/components/schemas/room" }
            }
          }
        }
      }
      #swagger.responses[401] = { description: "Token de acesso não fornecido" }
      #swagger.responses[403] = { description: "Token de acesso inválido" }
      #swagger.responses[500] = { description: "Erro interno do servidor" }
    */

    const rooms = await this.roomService.getAllRooms();
    res.status(200).json(rooms);
  }

  getRoomById = async (req, res) => {
    /*
      #swagger.tags = ['Room']
      #swagger.summary = 'Obtém um ambiente pelo ID'
      #swagger.description = 'Esse endpoint retorna um ambiente específico com base no ID fornecido.'
      #swagger.security = [{
        "bearerAuth": []
      }]
      #swagger.parameters['roomId'] = {
        in: 'path',
        required: true,
        type: 'string',
        description: 'ID do ambiente'
      }
      #swagger.responses[200] = {
        description: "Ambiente encontrado",
        content: {
          "application/json": {
            schema: { $ref: "#/components/schemas/room" }
          }
        }
      }
      #swagger.responses[400] = { description: "roomId é obrigatório" }
      #swagger.responses[401] = { description: "Token de acesso não fornecido" }
      #swagger.responses[403] = { description: "Token de acesso inválido" }
      #swagger.responses[404] = { description: "Sala não encontrada" }
      #swagger.responses[500] = { description: "Erro interno do servidor" }
    */

    const { roomId } = req.params;
    const room = await this.roomService.getRoomById(roomId);
    res.status(200).json(room);
  }

  getRoomByMicrocontrollerId = async (req, res) => {
    /*
      #swagger.tags = ['Room']
      #swagger.summary = 'Obtém um ambiente pelo ID do microcontrolador'
      #swagger.description = 'Esse endpoint retorna um ambiente específico com base no ID do microcontrolador fornecido.'
      #swagger.security = [{
        "bearerAuth": []
      }]
      #swagger.parameters['microcontrollerId'] = {
        in: 'path',
        required: true,
        type: 'string',
        description: 'ID do microcontrolador vinculado ao ambiente'
      }
      #swagger.responses[200] = {
        description: "Ambiente encontrado",
        content: {
          "application/json": {
            schema: { $ref: "#/components/schemas/room" }
          }
        }
      }
      #swagger.responses[400] = { description: "microcontrollerId é obrigatório" }
      #swagger.responses[401] = { description: "Token de acesso não fornecido" }
      #swagger.responses[403] = { description: "Token de acesso inválido" }
      #swagger.responses[404] = { description: "Sala não encontrada para o microcontrollerId fornecido" }
      #swagger.responses[500] = { description: "Erro interno do servidor" }
    */

    const { microcontrollerId } = req.params;
    const room = await this.roomService.getRoomByMicrocontrollerId(microcontrollerId);
    res.status(200).json(room);
  }

  updateRoom = async (req, res) => {
    /*
      #swagger.tags = ['Room']
      #swagger.summary = 'Atualiza um ambiente pelo ID'
      #swagger.description = 'Esse endpoint atualiza um ambiente específico com base no ID fornecido. Apenas administradores podem atualizar ambientes.'
      #swagger.security = [{
        "bearerAuth": []
      }]
      #swagger.parameters['roomId'] = {
        in: 'path',
        required: true,
        type: 'string',
        description: 'ID do ambiente'
      }
      #swagger.requestBody = {
        required: true,
        content: {
          "application/json": {
            schema: {
              type: "object",
              properties: {
                name: { type: "string", example: "Sala do Servidor" },
                microcontrollerId: { type: "string", example: "MCU123456" },
                minTemperature: { type: "number", example: 18 },
                maxTemperature: { type: "number", example: 27 },
                minHumidity: { type: "number", example: 30 },
                maxHumidity: { type: "number", example: 60 }
              }
            }
          }
        }
      }
      #swagger.responses[200] = {
        description: "Ambiente atualizado com sucesso",
        content: {
          "application/json": {
            schema: { $ref: "#/components/schemas/room" }
          }
        }
      }
      #swagger.responses[400] = { description: "Dados para atualização são obrigatórios" }
      #swagger.responses[401] = { description: "Token de acesso não fornecido" }
      #swagger.responses[403] = { description: "Acesso negado. Apenas administradores podem atualizar ambientes." }
      #swagger.responses[404] = { description: "Sala não encontrada para atualização" }
      #swagger.responses[500] = { description: "Erro interno do servidor" }
    */

    const { role } = req.user;
    if (role !== 'admin') {
      return res.status(403).json({ message: 'Acesso negado. Apenas administradores podem atualizar ambientes.' });
    }

    const { roomId } = req.params;
    const updateData = req.body;
    const updatedRoom = await this.roomService.updateRoom(roomId, updateData);
    res.status(200).json(updatedRoom);
  }

  deleteRoom = async (req, res) => {
    /*
      #swagger.tags = ['Room']
      #swagger.summary = 'Deleta um ambiente pelo ID'
      #swagger.description = 'Esse endpoint deleta um ambiente específico com base no ID fornecido. Apenas administradores podem deletar ambientes.'
      #swagger.security = [{
        "bearerAuth": []
      }]
      #swagger.parameters['roomId'] = {
        in: 'path',
        required: true,
        type: 'string',
        description: 'ID do ambiente'
      }
      #swagger.responses[204] = { description: "Ambiente deletado com sucesso" }
      #swagger.responses[400] = { description: "roomId é obrigatório" }
      #swagger.responses[401] = { description: "Token de acesso não fornecido" }
      #swagger.responses[403] = { description: "Acesso negado. Apenas administradores podem deletar ambientes." }
      #swagger.responses[404] = { description: "Sala não encontrada para exclusão" }
      #swagger.responses[500] = { description: "Erro interno do servidor" }
    */

    const { role } = req.user;
    if (role !== 'admin') {
      return res.status(403).json({ message: 'Acesso negado. Apenas administradores podem deletar ambientes.' });
    }

    const { roomId } = req.params;
    await this.roomService.deleteRoom(roomId);
    res.status(204).send();
  }

}

export default RoomController;
