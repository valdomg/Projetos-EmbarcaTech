import swaggerAutogen from 'swagger-autogen';

const swagger = swaggerAutogen({ openapi: '3.0.0' });

const doc = {
  info: {
    version: "1.0.0",
    title: "API de Monitoramento de Temperatura",
    description: "Essa API permite o monitoramento de temperatura em diferentes ambientes hospitalares utilizando IoT. Ela oferece endpoints para gerenciar usuários, ambientes e leituras de temperatura.",
  },
  servers: [
    {
      url: 'http://localhost:3000',
      description: 'Servidor Local',
    }
  ],
  components: {
    schemas: {
      user: {
        $name: "Jhon Doe",
        $email: "jhon.doe@example.com",
        $password: "SenhaSegura@123",
        role: "user"
      },
      temperature: {
        $temperature: 22.5,
        $humidity: 60,
        timestamp: "2023-10-01T12:00:00Z",
        $roomId: "64a7b2f5e4b0c8a1d2f3g4h5"
      },
      room: {
        $name: "Sala do servidor",
        $microcontrollerId: "MCU123456",
        minTemperature: 18,
        maxTemperature: 27,
        minHumidity: 30,
        maxHumidity: 60
      },
      report: {
        _id: "64a7b2f5e4b0c8a1d2f3g4h5",
        $room: "64a7b2f5e4b0c8a1d2f3g4h5",
        generatedAt: "2025-01-15T14:30:00Z",
        $filePath: "/public/reports/Relatorio-Sala-2025-01-01-2025-01-31-1710000000000.pdf"
      },
      error: {
        erro: "Mensagem de erro"
      }
    },
    securitySchemes: {
      bearerAuth: {
        type: 'http',
        scheme: 'bearer'
      }
    }
  }
};

const outputFile = './swagger-output.json';
const endpointsFiles = ['../routes.js'];

swagger(outputFile, endpointsFiles, doc).then(async () => {
  const { default: app } = await import('../app.js');
});
