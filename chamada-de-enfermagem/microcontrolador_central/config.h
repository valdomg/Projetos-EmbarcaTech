/**
 * @file config.h
 * @brief Arquivo central de configuração do firmware.
 *
 * @details
 * Este arquivo define constantes utilizadas em todo o sistema,
 * incluindo:
 *
 * - Configuração do sistema de log
 * - Definição de pinos de hardware
 * - Parâmetros de alerta sonoro
 * - Tópicos de comunicação MQTT
 * - Configurações da rede WiFi de acesso do dispositivo
 *
 * Essas definições são utilizadas por diferentes módulos do
 * firmware e centralizam os parâmetros que podem precisar
 * ser alterados durante o desenvolvimento ou implantação
 * do sistema.
 *
 * @note Alterações neste arquivo podem impactar diversos
 * módulos do sistema.
 *
 * @date 2026
 */

#ifndef CONFIG_HELPER_H
#define CONFIG_HELPER_H

#include "log.h"



/**
 * @brief Define o nível de verbosidade do sistema de log.
 *
 * @details
 * Controla quais mensagens serão exibidas no monitor serial.
 * Isso é útil para depuração e monitoramento do sistema.
 *
 * Níveis disponíveis (em ordem crescente de verbosidade):
 *
 * - LOG_NONE  : desativa logs
 * - LOG_ERROR : apenas erros críticos
 * - LOG_WARN  : avisos e erros
 * - LOG_INFO  : informações gerais do sistema
 * - LOG_DEBUG : mensagens detalhadas para depuração
 */
constexpr LogLevel LOG_MODE = LOG_DEBUG;



// -----------------------------------------------------------------------------
// Definição de pinos de hardware
// -----------------------------------------------------------------------------

/**
 * @brief Pino digital utilizado para controle do buzzer.
 *
 * @details
 * Este pino é configurado como saída e utilizado para
 * gerar alertas sonoros no sistema.
 */
constexpr uint8_t PIN_BUZZER = 15;


/**
 * @brief Pino digital utilizado para controle do LED indicador.
 *
 * @details
 * O LED pode ser utilizado para indicar estados do sistema,
 * como conexão WiFi, atividade do dispositivo ou condições
 * de alerta.
 */
constexpr uint8_t PIN_LED = 2;



// -----------------------------------------------------------------------------
// Configuração de alertas sonoros
// -----------------------------------------------------------------------------

/**
 * @brief Número máximo de ciclos de alerta sonoro emitidos.
 *
 * @details
 * Define quantas vezes o buzzer será ativado durante
 * um evento de alerta antes de ser automaticamente
 * desativado.
 */
constexpr uint8_t NUMBER_SOUND_ALERTS = 5;



// -----------------------------------------------------------------------------
// Configuração de tópicos MQTT
// -----------------------------------------------------------------------------

/**
 * @brief Tópico MQTT utilizado para publicação de eventos do dispositivo.
 */
constexpr const char* MQTT_PUBLICATION_TOPIC = "dispositivos/enfermaria";


/**
 * @brief Tópico MQTT utilizado para recebimento de comandos do sistema central.
 */
constexpr const char* MQTT_SUBSCRIPTION_TOPIC = "dispositivos/posto_enfermaria";


/**
 * @brief Tópico MQTT utilizado para envio de confirmação de mensagens.
 */
constexpr const char* MQTT_PUB_CONFIRMATION_TOPIC = "dispositivo/confirmacao";


/**
 * @brief Tópico MQTT utilizado para recebimento de confirmação do servidor.
 */
constexpr const char* MQTT_SUB_CONFIRMATION_TOPIC = "dispositivo/confirmacao/posto_enfermaria";


/**
 * @brief Porta padrão utilizada para comunicação com o broker MQTT.
 *
 * Valor padrão do protocolo MQTT: **1883**.
 */
constexpr int MQTT_PORT = 1883;



// -----------------------------------------------------------------------------
// Configuração da rede WiFi de acesso do dispositivo
// -----------------------------------------------------------------------------

/**
 * @brief Nome da rede WiFi criada pelo dispositivo (Access Point).
 *
 * @details
 * Esta rede é utilizada para acesso às configurações internas
 * do dispositivo, normalmente durante a etapa de configuração
 * inicial ou manutenção.
 */
constexpr const char* SSID_ACCESS_POINT = "xxxxxxxxxxxxx";


/**
 * @brief Senha da rede WiFi criada pelo dispositivo.
 *
 * @details
 * Utilizada para proteger o acesso ao ponto de acesso
 * gerado pelo microcontrolador.
 */
constexpr const char* PASSWORD_ACCESS_POINT = "xxxxxxxxxxxx";


#endif