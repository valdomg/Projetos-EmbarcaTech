import mongoose, { connect } from 'mongoose';
import 'dotenv/config';

connect(process.env.DATABASE_URL, {
  useNewUrlParser: true,
  useUnifiedTopology: true
})
  .then(() => console.log('Conectado ao Banco de dados!'))
  .catch(err => console.error('Erro ao conectar ao Banco de dados', err));
export default mongoose;