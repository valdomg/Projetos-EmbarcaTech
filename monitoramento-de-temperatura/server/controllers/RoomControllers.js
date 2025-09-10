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
          #swagger.description = 'Esse endpoint cria um novo ambiente no sistema.'
          #swagger.security = [{
            "bearerAuth": []
          }] 
          */

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
          */

        const { microcontrollerId } = req.params;
        const room = await this.roomService.getRoomByMicrocontrollerId(microcontrollerId);
        res.status(200).json(room);
    }

    updateRoom = async (req, res) => {
        /*
          #swagger.tags = ['Room']
          #swagger.summary = 'Atualiza um ambiente pelo ID'
          #swagger.description = 'Esse endpoint atualiza um ambiente específico com base no ID fornecido.'
          #swagger.security = [{
            "bearerAuth": []
          }] 
          */

        const { roomId } = req.params;
        const updateData = req.body;
        const updatedRoom = await this.roomService.updateRoom(roomId, updateData);
        res.status(200).json(updatedRoom);
    }

    deleteRoom = async (req, res) => {
        /*
          #swagger.tags = ['Room']
          #swagger.summary = 'Deleta um ambiente pelo ID'
          #swagger.description = 'Esse endpoint deleta um ambiente específico com base no ID fornecido.'
          #swagger.security = [{
            "bearerAuth": []
          }] 
          */
         const { roomId } = req.params;
         await this.roomService.deleteRoom(roomId);
         res.status(204).send();
    }

}

export default RoomController;