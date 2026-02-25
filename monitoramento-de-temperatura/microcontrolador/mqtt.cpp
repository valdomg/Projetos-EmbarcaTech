/**
 * @file mqtt.cpp
 * @brief Implementação do módulo de comunicação MQTT.
 *
 * Este módulo é responsável por gerenciar toda a comunicação entre o dispositivo
 * e o broker MQTT, incluindo:
 *
 * - Configuração da conexão com o broker;
 * - Gerenciamento de reconexão automática;
 * - Envio de dados de sensores em formato JSON;
 * - Reenvio de dados armazenados localmente em caso de falha;
 * - Publicação de alertas;
 * - Recuperação do cliente em caso de falhas críticas.
 *
 * A comunicação é baseada na biblioteca PubSubClient.
 *
 * O módulo foi projetado para operar de forma não bloqueante utilizando millis().
 */

#include "config.h"
#include "mqtt.h"
#include "log.h"
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "storage.h"
#include "config_storage.h"



// -----------------------------------------------------------------------------
// Objetos MQTT
// -----------------------------------------------------------------------------

/**
 * @brief Cliente de rede utilizado pelo MQTT.
 *
 * Responsável pela comunicação TCP/IP com o broker.
 *
 * @note Pode ser substituído por WiFiClientSecure caso TLS seja utilizado.
 */
WiFiClient espClient;


/**
 * @brief Cliente MQTT responsável pela comunicação com o broker.
 *
 * Utiliza o cliente de rede espClient como camada de transporte.
 */
PubSubClient client(espClient);


// -----------------------------------------------------------------------------
// Variáveis de controle de reconexão
// -----------------------------------------------------------------------------

/**
 * @brief Armazena o tempo da última tentativa de conexão MQTT.
 *
 * Utilizado para evitar tentativas excessivas de reconexão.
 */
static unsigned long lastAttempConnectMQTT = 0;


/**
 * @brief Intervalo mínimo entre tentativas de reconexão MQTT.
 *
 * Valor definido em milissegundos.
 */
static const unsigned long reconnectIntervalMQTT = 3000;


/**
 * @brief Armazena o tempo do último envio de dados armazenados.
 */
static unsigned long lastConnectionDataSent = 0;


/**
 * @brief Intervalo mínimo entre reenvio de mensagens armazenadas.
 */
static const unsigned long resendIntervalMQTT = 3000;


/**
 * @brief Indica se um alerta já foi enviado.
 *
 * Evita envio repetido da mesma mensagem de alerta.
 */
bool wasSendAlert = false;


// -----------------------------------------------------------------------------
// Inicialização
// -----------------------------------------------------------------------------

/**
 * @brief Inicializa o cliente MQTT.
 *
 * Configura:
 *
 * - Endereço do broker;
 * - Porta de comunicação;
 *
 * Esta função deve ser chamada durante o setup do sistema.
 *
 * @example
 * @code
 * setupMQTT();
 * @endcode
 */
void setupMQTT()
{
    client.setServer(cfg.mqttServer.c_str(), cfg.mqttPort);

    log(LOG_DEBUG, "MQTT inicializado");
}


/**
 * @brief Reinicializa o cliente MQTT e o cliente de rede.
 *
 * Utilizado quando ocorrem múltiplas falhas de conexão.
 *
 * Esta função recria:
 *
 * - Cliente de rede
 * - Cliente MQTT
 *
 * Permitindo restaurar a comunicação sem reiniciar o dispositivo.
 */
void resetMQTTClient()
{
    log(LOG_WARN, "Reinicializando cliente MQTT e TLS...");

    espClient = WiFiClient();

    client.setServer(cfg.mqttServer.c_str(), cfg.mqttPort);

    client.setClient(espClient);

    log(LOG_DEBUG, "Cliente MQTT reconfigurado");
}


// -----------------------------------------------------------------------------
// Gerenciamento de conexão
// -----------------------------------------------------------------------------

/**
 * @brief Verifica e mantém a conexão com o broker MQTT.
 *
 * Esta função:
 *
 * - Mantém a conexão ativa;
 * - Tenta reconectar automaticamente;
 * - Evita tentativas excessivas.
 *
 * Também executa:
 *
 * client.loop()
 *
 * necessário para manter a comunicação MQTT.
 *
 * @return true Cliente conectado
 * @return false Cliente desconectado
 *
 * @note Deve ser chamada continuamente dentro do loop()
 *
 * @example
 *
 * @code
 * if(checkMQTTConnected())
 * {
 *     publishSensorData(temp, hum);
 * }
 * @endcode
 */
bool checkMQTTConnected()
{
    if (client.connected())
    {
        client.loop();

        return true;
    }

    unsigned long now = millis();

    if (now - lastAttempConnectMQTT >= reconnectIntervalMQTT)
    {
        lastAttempConnectMQTT = now;

        log(LOG_DEBUG, "Tentando conectar ao MQTT...");

        if (client.connect(
                cfg.mqttDeviceId.c_str(),
                cfg.mqttUser.c_str(),
                cfg.mqttPass.c_str()))
        {
            log(LOG_INFO, "Conectado ao broker MQTT");

            return true;
        }
        else
        {
            log(LOG_ERROR, "Erro ao conectar com broker (rc=%d)", client.state());

            static uint8_t connection_drop_count = 0;

            connection_drop_count++;

            if (connection_drop_count >= 3)
            {
                resetMQTTClient();

                connection_drop_count = 0;
            }

            return false;
        }
    }

    return false;
}


// -----------------------------------------------------------------------------
// Publicação de dados
// -----------------------------------------------------------------------------

/**
 * @brief Publica dados de sensores no broker MQTT.
 *
 * Os dados são enviados no formato JSON.
 *
 * Estrutura:
 *
 * @code
 * {
 *   "microcontrollerId": "...",
 *   "temperature": 25.0,
 *   "humidity": 60.0
 * }
 * @endcode
 *
 * @param temperature Temperatura em graus Celsius
 * @param humidity Umidade relativa do ar
 *
 * @return true Sucesso
 * @return false Falha
 */
bool publishSensorData(float temperature, float humidity)
{
    StaticJsonDocument<128> doc;

    doc["microcontrollerId"] = cfg.mqttDeviceId.c_str();

    doc["temperature"] = temperature;

    doc["humidity"] = humidity;

    char buffer[128];

    serializeJson(doc, buffer);

    if (!client.publish(cfg.mqttTopicData.c_str(), buffer))
    {
        log(LOG_WARN, "Falha ao enviar dados ao broker MQTT");

        return false;
    }

    return true;
}


// -----------------------------------------------------------------------------
// Reenvio de dados armazenados
// -----------------------------------------------------------------------------

/**
 * @brief Reenvia dados armazenados localmente.
 *
 * Utilizado quando houve falha de comunicação anterior.
 *
 * Fluxo:
 *
 * - Verifica armazenamento;
 * - Envia mensagem;
 * - Remove mensagem enviada.
 *
 * @note Utiliza LittleFS como armazenamento.
 */
void resendMqttData()
{
    if (!hasDataStorage())
        return;

    unsigned long now = millis();

    if (now - lastConnectionDataSent >= resendIntervalMQTT)
    {
        lastConnectionDataSent = now;

        ObjectStorage obj = getObjectStorage();

        if (obj.valid)
        {
            if (publishSensorData(obj.temperature, obj.humidity))
            {
                deleteOneMessage();
            }
        }
    }
}


// -----------------------------------------------------------------------------
// Publicação de alertas
// -----------------------------------------------------------------------------

/**
 * @brief Publica mensagem de alerta no broker MQTT.
 *
 * Utilizado quando o sistema detecta condições críticas.
 *
 * O envio ocorre apenas uma vez até ser resetado.
 *
 * @param temperature Temperatura atual
 * @param humidity Umidade atual
 */
void publishAlert(float temperature, float humidity)
{
    if (!wasSendAlert)
    {
        StaticJsonDocument<128> doc;

        doc["microcontrollerId"] = cfg.mqttDeviceId.c_str();

        doc["temperature"] = temperature;

        doc["humidity"] = humidity;

        char buffer[128];

        serializeJson(doc, buffer);

        client.publish(cfg.mqttTopicAlert.c_str(), buffer);

        wasSendAlert = true;

        log(LOG_INFO, "alerta publicado no mqtt");
    }
}


/**
 * @brief Reseta o estado de envio de alerta.
 *
 * Permite que novos alertas sejam enviados.
 */
void resetStateSendAlert()
{
    wasSendAlert = false;
}