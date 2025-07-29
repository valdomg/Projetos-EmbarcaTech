import express from 'express';
import TemperatureController from './controllers/TemperatureControllers.js';

const router = express.Router();
const temperatureController = new TemperatureController();

router.get('/temperatures', temperatureController.getAllTemperatures);
router.get('/temperature/:id', temperatureController.getTemperatureById);

router.get('/room/:roomId/temperatures', temperatureController.getRoomTemperatures);
router.get('/room/:roomId/temperatures/interval', temperatureController.getRoomTemperaturesByInterval);
router.get('/temperatures/interval', temperatureController.getTemperatureReadingsByInterval);

export default router;