import 'dotenv/config';
import express from 'express';
import cors from 'cors';
import path from 'path';
import { fileURLToPath } from 'url';

import './mqtt/mqttclient.js';
import './database/db.js';
import temperatureRoutes from './routes.js';
import { errorHandler } from './middlewares/errorHandler.js';

import swaggerUi from 'swagger-ui-express';
import fs from "fs";

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const swaggerDocument = JSON.parse(
  fs.readFileSync(new URL("./docs/swagger-output.json", import.meta.url))
);

const app = express();

app.use(cors());
app.use(express.json());

// FRONTEND
app.use(express.static(path.join(__dirname, 'public')));

app.use('/api', temperatureRoutes);
app.use('/api/docs', swaggerUi.serve, swaggerUi.setup(swaggerDocument));
app.use((error, req, res, next) => errorHandler(error, req, res, next));

/*
app.get('/', (req, res) => {
  res.send('Servidor rodando!');
});
*/

const PORT = process.env.PORT || 3000;

app.listen(PORT, () => {
  console.log(`Servidor rodando em http://localhost:${PORT}`);
});

