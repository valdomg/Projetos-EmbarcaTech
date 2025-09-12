# API Nodejs de Monitoramento de Temperatura e Umidade em ambientes hospitalares

O sistema proposto visa automatizar o monitoramento de temperatura em ambientes hospitalares, como salas laboratoriais e áreas de armazenamento de medicamentos. Nesses locais, a manutenção de condições térmicas adequadas pode assegurar a qualidade e a segurança dos insumos, além de garantir a conformidade com os parâmetros estabelecidos pela vigilância sanitária.

Essa API Recebe dados de microcontroladores via **MQTT**, persiste no **MongoDB** e expõe endpoints REST

---

## 🚀 Tecnologias Principais

- **Node.js** + **Express** — servidor e rotas HTTP  
- **MongoDB** + **Mongoose** — persistência de dados  
- **MQTT.js** — comunicação com microcontroladores via MQTT  
- **jsonwebtoken (JWT)** — autenticação/autorização  
- **bcrypt** — hash de senhas  
- **swagger-autogen** + **swagger-ui-express** — documentação automática  
- **Jest** — testes  
- **dotenv** — variáveis de ambiente  
- **cross-env** — compatibilidade de scripts entre sistemas operacionais

---

## 📂 Arquitetura do Projeto

Separação por camadas / responsabilidades:

```
controllers/         # Controladores das rotas
├── AuthControllers.js
├── RoomControllers.js
├── TemperatureControllers.js
└── UserControllers.js

database/            # Conexão com o MongoDB
└── db.js

docs/                # Arquivos referentes ao Swagger - documentação da api
├── swagger-output.json
└── swagger.js

middlewares/         # Middlewares
├── auth.js
└── errorHandler.js

models/              # Schemas / Models Mongoose
├── Room.js
├── Temperature.js
└── User.js

mqtt/                # Cliente / configuração MQTT
└── mqttClient.js

services/            # Regras de negócio (cada pasta contém service + testes)
├── auth/
│   ├── AuthServices.js
│   └── AuthServices.spec.js
├── Room/
│   ├── RoomServices.js
│   └── RoomServices.spec.js
├── TemperatureReading/
│   ├── TemperatureReadingServices.js
│   └── TemperatureReadingServices.spec.js
└── User/
    ├── UserServices.js
    └── UserServices.spec.js

utils/               # Helpers / utilitários
└── errors.js         # ApiError, helpers de erro

app.js               # Inicialização da aplicação
routes.js            # Definição das rotas
jest.config.mjs      # Configuração do Jest para testes
.env.example         # Exemplo de variáveis de ambiente
package.json         # Dependências e scripts
```

> Observação: dentro de `services/` cada domínio tem um arquivo de service (ex.: `AuthServices.js`) e seu teste (`AuthServices.spec.js`).

---

## ⚙️ Variáveis de Ambiente

Crie um arquivo `.env` na raiz do projeto (baseado em `.env.example`). Exemplo:

```env
# string de conexão com MongoDB (ajuste a porta se necessário)
DATABASE_URL=mongodb://localhost:27017/database_name

# porta que a API vai escutar
PORT=3000

# broker MQTT usado pelos microcontroladores
MQTT_BROKER_URL=mqtt://host:port

# segredo para assinar JWTs
JWT_SECRET=chave_secreta_jwt
```

---

## 📖 Documentação da API (Swagger)

A documentação é gerada com **swagger-autogen** e servida via **swagger-ui-express**.

### Gerar/Atualizar documentação
```bash
npm run swagger
```

### Acessar no navegador
```
http://localhost:3000/api/docs
```

---

## 🛠️ Como Instalar e Rodar

1. **Clone o repositório**
```bash
git clone https://github.com/Embarcatech-Residencia-CAPEDII/Projetos
cd Projetos/monitoramento-de-temperatura/server
```

2. **Instale as dependências**
```bash
npm install
```

3. **Configure as variáveis de ambiente**
- Crie o arquivo `.env` com os valores do exemplo acima.

4. **Rode a aplicação**
```bash
npm start
```
> Observação: Para o projeto funcionar corretamente é necessario um broker mqtt já configurado e o mongodb devidamente instalado.

---

## 🧪 Testes

Testes com **Jest**. Para rodar:
```bash
npm test
```

---

## 📌 Endpoints Principais

### 🔑 Auth
- `POST /login` → Autenticação de usuários

### 👤 Users
- `POST /user` → Cria usuário  
- `GET /users` → Lista todos os usuários  
- `GET /user/{email}` → Busca usuário por email  
- `PUT /user/{id}` → Atualiza usuário  
- `DELETE /user/{id}` → Remove usuário

### 🏠 Room
- `POST /room` → Cria ambiente  
- `GET /rooms` → Lista todos os ambientes  
- `GET /room/id/{roomId}` → Busca por ID  
- `GET /room/microcontroller/{microcontrollerId}` → Busca por microcontrolador  
- `PUT /room/{roomId}` → Atualiza ambiente  
- `DELETE /room/{roomId}` → Remove ambiente

### 🌡️ Temperature
- `GET /temperatures` → Lista todas as leituras  
- `GET /temperature/{id}` → Busca leitura por ID  
- `GET /temperatures/interval` → Leituras por intervalo  
- `GET /room/{roomId}/temperatures` → Leituras de um ambiente  
- `GET /room/{roomId}/temperatures/interval` → Leituras por intervalo de um ambiente

---

## 🧾 Scripts úteis

- `npm start` — inicia a aplicação  
- `npm run swagger` — gera a documentação Swagger  
- `npm test` — executa os testes com Jest