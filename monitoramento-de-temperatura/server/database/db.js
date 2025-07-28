import mongoose, { connect } from 'mongoose';
import 'dotenv/config';

connect(process.env.DATABASE_URL, {
  useNewUrlParser: true,
  useUnifiedTopology: true
})
  .then(() => console.log('Successfully connected to the database!'))
  .catch(err => console.error('Error connecting to the database', err));
export default mongoose;