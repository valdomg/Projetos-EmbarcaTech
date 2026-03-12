import mongoose from "mongoose";

const reportSchema = new mongoose.Schema({
  room: {
    type: mongoose.Schema.Types.ObjectId,
    ref: "Room",
    required: true
  },
  generatedAt: {
    type: Date,
    default: Date.now
  },
  filePath: {
    type: String,
    required: true
  }
});

const Report = mongoose.model('Report', reportSchema);
export default Report;
