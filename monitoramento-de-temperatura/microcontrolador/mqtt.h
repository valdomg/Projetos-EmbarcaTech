#ifndef MQTT_HELPER_H
#define MQTT_HELPER_H

#include <PubSubClient.h>

/**
 * @file mqtt.h
 * @brief Interface pública do módulo de comunicação MQTT.
 *
 * Este módulo fornece os recursos necessários para comunicação entre o
 * microcontrolador e um broker MQTT.
 *
 * Funcionalidades principais:
 *
 * - Inicialização da conexão MQTT;
 * - Gerenciamento automático de reconexão;
 * - Publicação de dados de sensores;
 * - Publicação de mensagens de alerta;
 * - Reenvio automático de mensagens armazenadas localmente.
 *
 * O envio de dados é realizado utilizando payloads no formato JSON.
 *
 * @note A implementação está localizada em mqtt.cpp
 */


// -----------------------------------------------------------------------------
// Variáveis globais
// -----------------------------------------------------------------------------

/**
 * @brief Cliente MQTT global.
 *
 * Responsável pela comunicação com o broker MQTT.
 *
 * Este objeto é:
 *
 * - Inicializado em mqtt.cpp
 * - Utilizado internamente pelas funções deste módulo
 *
 * @note Pode ser utilizado diretamente em casos avançados.
 *
 * @warning Evite modificar suas configurações fora do módulo MQTT.
 */
extern PubSubClient client;


// -----------------------------------------------------------------------------
// Funções públicas
// -----------------------------------------------------------------------------

/**
 * @brief Inicializa o cliente MQTT.
 *
 * Configura:
 *
 * - Endereço do broker;
 * - Porta de comunicação;
 * - Cliente de rede associado.
 *
 * Deve ser chamada uma vez durante a inicialização do sistema.
 *
 * @example
 *
 * @code
 * void setup()
 * {
 *     setupMQTT();
 * }
 * @endcode
 */
void setupMQTT();


/**
 * @brief Verifica e mantém a conexão com o broker MQTT.
 *
 * Esta função:
 *
 * - Mantém a conexão ativa;
 * - Executa client.loop();
 * - Realiza reconexão automática se necessário.
 *
 * @return true Cliente conectado
 * @return false Cliente desconectado
 *
 * @note Deve ser chamada continuamente dentro do loop principal.
 *
 * @example
 *
 * @code
 * void loop()
 * {
 *     checkMQTTConnected();
 * }
 * @endcode
 */
bool checkMQTTConnected();


/**
 * @brief Publica dados de sensores no broker MQTT.
 *
 * Os dados são enviados no formato JSON contendo:
 *
 * - Identificador do dispositivo
 * - Temperatura
 * - Umidade
 *
 * Exemplo de payload:
 *
 * @code
 * {
 *   "microcontrollerId": "device01",
 *   "temperature": 25.4,
 *   "humidity": 60.1
 * }
 * @endcode
 *
 * @param temperature Temperatura em graus Celsius.
 *
 * @param humidity Umidade relativa em porcentagem.
 *
 * @return true Sucesso no envio
 *
 * @return false Falha no envio
 *
 * @note Em caso de falha, os dados podem ser armazenados localmente.
 */
bool publishSensorData(float temperature, float humidity);


/**
 * @brief Reenvia mensagens armazenadas localmente.
 *
 * Esta função:
 *
 * - Verifica se existem mensagens armazenadas;
 * - Envia uma mensagem por vez;
 * - Remove mensagens enviadas com sucesso.
 *
 * Utilizado para garantir integridade dos dados quando há falha de conexão.
 *
 * @note Deve ser chamada periodicamente dentro do loop principal.
 */
void resendMqttData();


/**
 * @brief Publica mensagem de alerta no broker MQTT.
 *
 * Utilizado quando o sistema detecta condições críticas.
 *
 * Exemplo de uso:
 *
 * @code
 * publishAlert(temp, hum);
 * @endcode
 *
 * @param temperature Temperatura atual
 *
 * @param humidity Umidade atual
 *
 * @note O alerta é enviado apenas uma vez até ser resetado.
 */
void publishAlert(float temperature, float humidity);


/**
 * @brief Reseta o estado de envio de alerta.
 *
 * Permite que novos alertas sejam enviados.
 *
 * Deve ser chamado quando o sistema retorna ao estado normal.
 *
 * @example
 *
 * @code
 * resetStateSendAlert();
 * @endcode
 */
void resetStateSendAlert();


#endif