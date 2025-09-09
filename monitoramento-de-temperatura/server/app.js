import 'dotenv/config';
import express from 'express';
import './mqtt/mqttclient.js';
import './database/db.js';
import temperatureRoutes from './routes.js';
import { errorHandler } from './middlewares/errorHandler.js';

const app = express();
app.use(express.json());
app.use('/api', temperatureRoutes);
app.use((error, req, res, next) => errorHandler(error, req, res, next));

app.get('/', (req, res) => {
  res.send('Servidor rodando!');
});


const PORT = process.env.PORT || 3000;

app.listen(PORT, () => {
  console.log(`Servidor rodando na porta ${PORT}`);
});

