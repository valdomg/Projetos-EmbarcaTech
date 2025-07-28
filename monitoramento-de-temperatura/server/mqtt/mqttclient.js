import mqtt from "mqtt";

const connectUrl = process.env.MQTT_BROKER_URL;
const clientId = `mqtt_${Math.random().toString(16).slice(3)}`;

const mqttClient = mqtt.connect(connectUrl, {
  clientId,
  clean: true,
  connectTimeout: 4000,
  reconnectPeriod: 1000,
})

mqttClient.on('connect', () => {
  console.log('Connected')
})

mqttClient.on('message', (topic, message) => {
  console.log(`Received message on topic ${topic}:`, message.toString());
});


mqttClient.on('error', (err) => {
  console.error('MQTT connection error:', err);
});

mqttClient.on('reconnect', () => {
  console.log('MQTT reconnecting...');
});

export default mqttClient;