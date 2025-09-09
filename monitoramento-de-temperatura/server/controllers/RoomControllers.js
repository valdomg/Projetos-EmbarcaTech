import RoomService from '../services/Room/RoomServices.js';
import RoomModel from '../models/Room.js';

class RoomController {
    constructor() {
        this.roomService = new RoomService(RoomModel);
    }

    createRoom = async (req, res) => {
        const room = await this.roomService.createRoom(req.body);
        res.status(201).json(room);
    }

    getAllRooms = async (req, res) => {
        const rooms = await this.roomService.getAllRooms();
        res.status(200).json(rooms);
    }

}

export default RoomController;