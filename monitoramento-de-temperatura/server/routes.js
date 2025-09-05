import express from 'express';
import TemperatureController from './controllers/TemperatureControllers.js';
import RoomController from './controllers/RoomControllers.js';
import UserController from './controllers/UserControllers.js';

const router = express.Router();
const temperatureController = new TemperatureController();
const roomController = new RoomController();
const userController = new UserController();

router.get('/temperatures', temperatureController.getAllTemperatures);
router.get('/temperature/:id', temperatureController.getTemperatureById);
router.get('/temperatures/interval', temperatureController.getTemperatureReadingsByInterval);


router.post('/room', roomController.createRoom);
router.get('/rooms', roomController.getAllRooms);
router.get('/room/:roomId/temperatures', temperatureController.getRoomTemperatures);
router.get('/room/:roomId/temperatures/interval', temperatureController.getRoomTemperaturesByInterval);

router.post('/user', userController.createUser);
router.get('/user/:email', userController.getUserByEmail);

export default router;