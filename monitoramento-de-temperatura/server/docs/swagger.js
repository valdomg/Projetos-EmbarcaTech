import swaggerAutogen from 'swagger-autogen';

const swagger = swaggerAutogen({ openapi: '3.0.0' });

const doc = {
  info: {
    version: "1.0.0",
    title: "My API",
    description: "Some description..."
  },
  servers: [
    {
      url: 'http://localhost:5000'
    }
  ],
  components: {
    schemas: {
      someBody: {
        $name: "Jhon Doe",
        $age: 29,
        about: ""
      },
      someResponse: {
        name: "Jhon Doe",
        age: 29,
      },
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
