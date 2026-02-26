import express from 'express';
import TemperatureController from './controllers/TemperatureControllers.js';
import RoomController from './controllers/RoomControllers.js';
import UserController from './controllers/UserControllers.js';
import AuthController from './controllers/AuthControllers.js';
import { authenticate } from './middlewares/auth.js';

const router = express.Router();
const temperatureController = new TemperatureController();
const roomController = new RoomController();
const userController = new UserController();
const authController = new AuthController();

router.post("/login", authController.signIn);

router.get('/temperatures', authenticate, temperatureController.getAllTemperatures);
router.get('/temperature/:id', authenticate, temperatureController.getTemperatureById);
router.get('/temperatures/interval', authenticate, temperatureController.getTemperatureReadingsByInterval);


router.post('/room', authenticate, roomController.createRoom);
router.get('/rooms', authenticate, roomController.getAllRooms);
router.get('/room/:roomId/temperatures', authenticate, temperatureController.getRoomTemperatures);
router.get('/room/:roomId/temperatures/interval', authenticate, temperatureController.getRoomTemperaturesByInterval);
router.get('/room/current', authenticate, temperatureController.getCurrentTemperature);
router.get('/room/:roomId/report', authenticate, temperatureController.getReport);
router.get('/room/:roomId/pdf', authenticate, temperatureController.getPdfReport);
router.get('/room/id/:roomId', authenticate, roomController.getRoomById);
router.get('/room/microcontroller/:microcontrollerId', authenticate, roomController.getRoomByMicrocontrollerId);
router.patch('/room/:roomId', authenticate, roomController.updateRoom);
router.delete('/room/:roomId', authenticate, roomController.deleteRoom);

router.post('/user', authenticate, userController.createUser);
router.get('/users', authenticate, userController.getAllUsers);
router.get('/user/:email', authenticate, userController.getUserByEmail);
router.patch('/user/:userId', authenticate, userController.updateUser);
router.delete('/user/:userId', authenticate, userController.deleteUser);
router.patch('/user/:userId/change-password', authenticate, userController.changePassword);

export default router;