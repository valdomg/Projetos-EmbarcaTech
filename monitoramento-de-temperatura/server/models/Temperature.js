import mongoose from "mongoose";

const temperatureSchema = new mongoose.Schema({
    temperature: {
        type: Number,
        required: true
    },
    humidity: {
        type: Number,
        required: true
    },
    timestamp: {
        type: Date,
        default: Date.now
    },
});

const Temperature = mongoose.model('Temperature', temperatureSchema);
export default Temperature;