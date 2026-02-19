import mongoose from "mongoose";

const roomSchema = new mongoose.Schema({
  name: {
    type: String,
    required: true
  },
  microcontrollerId: {
    type: String,
    required: true,
    unique: true
  },
  minTemperature: {
    type: Number,
    required: true,
    default: 10
  },
  maxTemperature: {
    type: Number,
    required: true,
    default: 30
  },
  minHumidity: {
    type: Number,
    required: true,
    default: 30
  },
  maxHumidity: {
    type: Number,
    required: true,
    default: 60
  }
});

const Room = mongoose.model("Room", roomSchema);
export default Room;