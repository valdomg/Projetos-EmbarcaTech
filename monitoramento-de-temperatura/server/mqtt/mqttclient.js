import mqtt from "mqtt";
import TemperatureModel from '../models/Temperature.js';
import TemperatureService from '../services/TemperatureReading/TemperatureServices.js';

const connectUrl = process.env.MQTT_BROKER_URL;
const clientId = `mqtt_${Math.random().toString(16).slice(3)}`;

const temperatureService = new TemperatureService(TemperatureModel);

const mqttClient = mqtt.connect(connectUrl, {
  clientId,
  clean: true,
  connectTimeout: 4000,
  reconnectPeriod: 1000,
})

mqttClient.on('connect', () => {
  console.log('Connected');
  mqttClient.subscribe('sensors/temperature', (err) => {
    if (err) {
      console.error('Subscription error:', err);
    } else {
      console.log('Subscribed to sensor/temperature');
    }
  });
})

mqttClient.on('message', (topic, message) => {
  console.log(`Received message on topic ${topic}:`, message.toString());

  if (topic === 'sensors/temperature') {
    const temperatureData = JSON.parse(message.toString());
    temperatureService.createTemperatureReading(temperatureData);
  }
});


mqttClient.on('error', (err) => {
  console.error('MQTT connection error:', err);
});

mqttClient.on('reconnect', () => {
  console.log('MQTT reconnecting...');
});

export default mqttClient;