/**
 * @file mqtt.cpp
 * @brief Implementação do módulo de comunicação MQTT do sistema.
 *
 * @details
 * Este módulo é responsável por gerenciar toda a comunicação MQTT do dispositivo,
 * incluindo:
 *
 * - Conexão com o broker MQTT
 * - Reconexão automática em caso de falha
 * - Recebimento de mensagens através de callback
 * - Processamento de mensagens JSON recebidas
 * - Publicação de mensagens de confirmação
 *
 * O módulo utiliza a biblioteca PubSubClient para comunicação MQTT e
 * WiFiClientSecure para estabelecer conexões seguras via TLS/SSL.
 *
 * Fluxo principal de funcionamento:
 *
 * 1. setupMQTT() configura o cliente MQTT e registra o callback.
 * 2. checkMQTTConnected() mantém a conexão ativa e tenta reconectar se necessário.
 * 3. Quando uma mensagem chega, a função callback() é executada.
 * 4. O JSON recebido é processado por processing_json_MQTT().
 * 5. Após processamento, o dispositivo envia uma confirmação ao broker.
 *
 * Além disso, o módulo também:
 * - Ativa um alerta sonoro ao receber uma nova chamada.
 * - Controla o recebimento de confirmações do servidor.
 *
 * Este módulo faz parte do sistema de chamadas de enfermagem baseado em MQTT.
 */

#include <WiFiClientSecure.h>    // Biblioteca para conexões seguras via TLS/SSL (necessária para MQTT com SSL/TLS).
#include "mqtt.h"                // Header com as declarações das funções e variáveis MQTT usadas no projeto.
#include "config.h"              // Arquivo de configuração com constantes (servidor, usuário, senha, tópicos, etc.).
#include "jsonDataProcessing.h"  // Header com a declaração da função que processa os dados JSON recebido do MQTT
#include "listNursingCall_utils.h"
#include "log.h"
#include "buzzer.h"
#include "config_storage.h"

// -----------------------------------------------------------------------------
// Objetos globais de comunicação
// -----------------------------------------------------------------------------

/**
 * @brief Cliente de rede utilizado para comunicação com o broker MQTT.
 *
 * Este objeto gerencia a conexão TCP/IP através da rede Wi-Fi e é utilizado
 * pelo cliente MQTT como camada de transporte para envio e recebimento
 * de mensagens.
 */
WiFiClient espClient;  // Cliente Wi-Fi seguro (usado pelo PubSubClient para comunicação MQTT).

/**
 * @brief Cliente MQTT responsável pela comunicação com o broker.
 *
 * Utiliza o objeto espClient como interface de transporte para a rede.
 * Esse cliente gerencia:
 * - conexão com o broker MQTT
 * - envio de mensagens
 * - recebimento de mensagens
 * - manutenção da sessão MQTT
 */
PubSubClient client(espClient);  // Cliente MQTT que usa o cliente Wi-Fi seguro como transporte.

// -----------------------------------------------------------------------------
// Variáveis de controle da conexão MQTT
// -----------------------------------------------------------------------------

/**
 * @brief Armazena o instante da última tentativa de conexão ao broker MQTT.
 *
 * Essa variável é utilizada para controlar o intervalo entre tentativas
 * de reconexão, evitando que o dispositivo tente conectar continuamente
 * ao servidor.
 */
static unsigned long lastAttempConnectMQTT = 0;  // Guarda o tempo da última tentativa de conexão com o broker.

/**
 * @brief Intervalo mínimo entre tentativas de reconexão MQTT.
 *
 * Valor em milissegundos que define quanto tempo o sistema deve esperar
 * antes de tentar se reconectar novamente ao broker MQTT.
 */
static const unsigned long reconnectIntervalMQTT = 3000;  // Intervalo (ms) entre tentativas de reconexão ao broker.

// -----------------------------------------------------------------------------
// Variáveis de estado do sistema
// -----------------------------------------------------------------------------

/**
 * @brief Indica que uma mensagem de confirmação foi recebida do servidor MQTT.
 *
 * Quando o servidor confirma o processamento de uma chamada, essa flag
 * é ativada para que o sistema possa tratar a confirmação e atualizar
 * o estado interno da aplicação, deletando a chamada que foi atendida.
 */
bool hasOKMessage = false;

/**
 * @brief Armazena o identificador da chamada que deve ser removida.
 *
 * O valor é extraído da mensagem de confirmação recebida via MQTT.
 * Após o processamento da confirmação, a chamada correspondente
 * poderá ser removida da lista local de chamadas de enfermagem.
 */
const char* calledToBeErased;

volatile bool newMessage = false;

char receivedTopic[MAX_TOPIC_SIZE];
char receivedPayload[MAX_PAYLOAD_SIZE];
unsigned int receivedLength = 0;

// -----------------------------------------------------------------------------
// Funções auxiliares
// -----------------------------------------------------------------------------

/**
 * @brief Reconfigura o cliente MQTT e o cliente TLS (WiFiClientSecure).
 *
 * Essa função é chamada quando o cliente MQTT falha repetidamente ao tentar
 * reconectar ao broker. Em situações assim, o cliente TLS pode ficar em um
 * estado inválido, bloqueando novas conexões. O procedimento abaixo recria
 * ambos os clientes para restaurar a comunicação.
 *
 * Operações executadas:
 * - Reinstancia o objeto `WiFiClientSecure` (limpa conexões antigas);
 * - Redefine o servidor MQTT e a porta segura;
 * - Reassocia o cliente TLS ao cliente MQTT.
 *
 * Essa rotina evita a necessidade de reiniciar o microcontrolador.
 */
void resetMQTTClient() {
  log(LOG_WARN, "Reinicializando cliente MQTT e TLS...");

  espClient = WiFiClient();  // recria o cliente seguro
  client.setServer(cfg.mqttServer.c_str(), cfg.mqttPort);
  client.setClient(espClient);

  log(LOG_DEBUG, "Cliente MQTT reconfigurado");
}



// -----------------------------------------------------------------------------
// Funções principais
// -----------------------------------------------------------------------------

/**
 * @brief Configura a conexão com o servidor MQTT.
 * 
 * Define parâmetros básicos como servidor, porta e função de callback
 * que será chamada ao receber mensagens.
 */
void setupMQTT() {
  // espClient.setInsecure();              // Desabilita verificação de certificado SSL (não verifica autenticidade).
  client.setServer(cfg.mqttServer.c_str(), cfg.mqttPort);  // Define o servidor MQTT e a porta (8883 = padrão para MQTTs).
  client.setCallback(callback);                            // Registra a função callback para mensagens recebidas.
}

/**
 * @brief Verifica a conexão com o broker MQTT e reconecta se necessário.
 * 
 * Esta função deve ser chamada periodicamente dentro do loop principal
 * do firmware.
 * 
 * Funções executadas:
 * - Mantém a comunicação MQTT ativa (client.loop()).
 * - Verifica se o cliente ainda está conectado.
 * - Caso a conexão tenha sido perdida, tenta reconectar após um intervalo.
 */
bool checkMQTTConnected() {
  client.loop();                   // Mantém a comunicação ativa e processa mensagens recebidas.
  if (client.connected()) return true;  // Se já está conectado, sai da função.

  unsigned long now = millis();  // Captura o tempo atual (ms desde o boot).

  // Só tenta reconectar se já passou o intervalo configurado
  if (now - lastAttempConnectMQTT >= reconnectIntervalMQTT) {
    lastAttempConnectMQTT = now;  // Atualiza o tempo da última tentativa

    log(LOG_INFO, "Tentando conectar ao MQTT");
    // Tenta conectar ao broker usando credenciais do config.h
    if (client.connect(cfg.mqttDeviceId.c_str(), cfg.mqttUser.c_str(), cfg.mqttPass.c_str())) {
      log(LOG_INFO, "Conectado ao broker!");
      char fullTopic[50];
      snprintf(fullTopic, sizeof(fullTopic), "%s/%s", MQTT_SUBSCRIPTION_TOPIC, "#");
      client.subscribe(fullTopic);  // Inscreve-se no tópico para receber mensagens
      client.subscribe(MQTT_SUB_CONFIRMATION_TOPIC);

      return true;
    } else {
      log(LOG_ERROR, "Falha na conexão com mqtt, rc= %d", client.state());  // Mostra o código de erro da conexão

      static uint8_t connection_drop_count = 0;

      connection_drop_count++;

      if (connection_drop_count >= 3) {
        resetMQTTClient();
        connection_drop_count = 0;
      }

      return false;
    }
  }
  return false;
}

/**
 * @brief Callback executada quando uma mensagem MQTT é recebida.
 *
 * @param topic   Tópico no qual a mensagem foi publicada.
 * @param payload Dados da mensagem recebida.
 * @param length  Tamanho do payload recebido.
 *
 * Comportamento da função:
 *
 * 1. Se a mensagem for de confirmação:
 *    - Extrai o ID da chamada confirmada.
 *    - Sinaliza que essa chamada deve ser removida da lista.
 *
 * 2. Se a mensagem for uma nova chamada:
 *    - Processa o JSON recebido.
 *    - Envia uma confirmação ao servidor MQTT.
 *    - Ativa o alerta sonoro.
 */
void callback(char* topic, byte* payload, unsigned int length) {
  // log(LOG_DEBUG, "Mensagem recebida no tópico: %s", topic);

  if (length >= MAX_PAYLOAD_SIZE || length == 0) return;

  // copia payload
  memcpy(receivedPayload, payload, length);
  receivedPayload[length] = '\0';

  // copia tópico
  strncpy(receivedTopic, topic, MAX_TOPIC_SIZE - 1);
  receivedTopic[MAX_TOPIC_SIZE - 1] = '\0';

  receivedLength = length;
  newMessage = true;
  
}

/**
 * @brief Publica uma mensagem MQTT para um dispositivo específico.
 *
 * O tópico final é formado concatenando:
 *    tópico_base + "/" + deviceId
 *
 * @param deviceId ID do dispositivo associado à mensagem.
 * @param topic    Tópico base de publicação.
 * @param message  Conteúdo da mensagem a ser enviada.
 * @param retained Define se a mensagem deve ser mantida no broker.
 *
 * @return true se a mensagem foi publicada com sucesso.
 * @return false se ocorreu falha na publicação.
 */
bool publicReponseDivice(const char* deviceId, const char* topic, const char* message, bool retained) {

  char fullTopic[50];
  snprintf(fullTopic, sizeof(fullTopic), "%s/%s", topic, deviceId);  // Monta o tópico final (base + id).


  // char buffer[256];
  if (!client.publish(fullTopic, message, retained)) {
    log(LOG_WARN, "Falha ao enviar dados ao broker MQTT");
    return false;
  }

  return true;
}
