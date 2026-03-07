#ifndef MQTT_HELPER_H
#define MQTT_HELPER_H

/**
 * @file mqtt.h
 * @brief Interface do módulo de comunicação MQTT do dispositivo.
 *
 * @details
 * Este módulo fornece as funções e variáveis necessárias para gerenciar
 * a comunicação MQTT do sistema, incluindo:
 *
 * - configuração da conexão com o broker MQTT
 * - manutenção da conexão
 * - recebimento de mensagens através de callback
 * - publicação de mensagens no broker
 *
 * O módulo utiliza a biblioteca PubSubClient para implementação
 * do protocolo MQTT.
 */

#include <PubSubClient.h>   ///< Biblioteca que implementa o cliente MQTT (conexão, publish, subscribe).

// -----------------------------------------------------------------------------
// Declarações globais
// -----------------------------------------------------------------------------

/**
 * @brief ID da chamada que deve ser removida após confirmação do servidor.
 *
 * Essa variável é atualizada quando uma mensagem de confirmação é recebida
 * do broker MQTT. O valor representa o identificador da chamada que deve
 * ser removida da lista local do dispositivo.
 */
extern const char* calledToBeErased;

/**
 * @brief Indica que foi recebida uma mensagem de confirmação do servidor.
 *
 * Quando uma confirmação é recebida via MQTT, essa flag é ativada
 * para que o sistema possa processar a remoção da chamada correspondente.
 */
extern bool hasOKMessage;

/**
 * @brief Cliente MQTT global utilizado pelo sistema.
 *
 * Esse objeto gerencia:
 * - conexão com o broker
 * - envio de mensagens (publish)
 * - recebimento de mensagens (subscribe)
 * - manutenção da sessão MQTT
 *
 * A definição do objeto está localizada no arquivo `mqtt.cpp`.
 */
extern PubSubClient client;


// -----------------------------------------------------------------------------
// Assinaturas das funções
// -----------------------------------------------------------------------------

/**
 * @brief Inicializa e configura o cliente MQTT.
 *
 * Define o servidor MQTT, a porta de comunicação e registra
 * a função callback responsável por tratar mensagens recebidas.
 *
 * Esta função deve ser chamada durante a inicialização do sistema.
 */
void setupMQTT();

/**
 * @brief Verifica e mantém a conexão com o broker MQTT.
 *
 * Essa função deve ser chamada periodicamente no loop principal
 * do programa. Ela executa:
 *
 * - manutenção da comunicação MQTT (client.loop())
 * - verificação do estado da conexão
 * - tentativa automática de reconexão caso a conexão seja perdida
 */
void checkMQTTConnected();

/**
 * @brief Callback executada quando uma mensagem MQTT é recebida.
 *
 * Esta função é chamada automaticamente pela biblioteca MQTT
 * sempre que uma nova mensagem chega em um tópico inscrito.
 *
 * @param topic   Tópico onde a mensagem foi publicada.
 * @param payload Ponteiro para os dados recebidos.
 * @param length  Tamanho do payload em bytes.
 */
void callback(char* topic, byte* payload, unsigned int length);

/**
 * @brief Publica uma mensagem em um tópico MQTT específico do dispositivo.
 *
 * O tópico final é montado dinamicamente concatenando:
 *
 * `topic_base/deviceId`
 *
 * @param deviceId Identificador do dispositivo associado à mensagem.
 * @param topic    Tópico base de publicação.
 * @param message  Conteúdo da mensagem a ser enviada.
 * @param retained Define se a mensagem deve ser mantida no broker.
 *
 * @return true se a publicação foi realizada com sucesso.
 * @return false caso ocorra falha no envio da mensagem.
 */
bool publicReponseDivice(const char* deviceId, const char* topic, const char* message, bool retained = false);

#endif // MQTT_HELPER_H