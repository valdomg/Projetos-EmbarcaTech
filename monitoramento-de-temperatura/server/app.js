import 'dotenv/config';
import express from 'express';
import mqttClient from './mqtt/mqttclient.js';
const app = express();
app.use(express.json());

app.get('/', (req, res) => {
  res.send('Servidor rodando!');
});

const PORT = process.env.PORT || 3000;

app.listen(PORT, () => {
  console.log(`Servidor rodando na porta ${PORT}`);
});

