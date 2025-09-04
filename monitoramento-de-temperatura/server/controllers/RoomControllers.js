import RoomService from '../services/Room/RoomServices.js';
import RoomModel from '../models/Room.js';

class RoomController {
    constructor() {
        this.roomService = new RoomService(RoomModel);
    }

    createRoom = async (req, res) => {
        try {
            const room = await this.roomService.createRoom(req.body);
            res.status(201).json(room);
        } catch (error) {
            res.status(500).json({ error: error.message });
        }
    }

    getAllRooms = async (req, res) => {
        try {
            const rooms = await this.roomService.getAllRooms();
            res.status(200).json(rooms);
        } catch (error) {
            res.status(500).json({ error: error.message });
        }
    }

}

export default RoomController;