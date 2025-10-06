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
      url: 'http://localhost:5000',
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
        min_temperature: 18,
        max_temperature: 27
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
