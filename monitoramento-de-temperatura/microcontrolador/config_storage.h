/**
 * @file config_storage.h
 * @brief Interface do subsistema de armazenamento persistente de configuração.
 *
 * @details
 * Este módulo define a estrutura de dados e as funções responsáveis por
 * gerenciar as configurações persistentes do sistema.
 *
 * As configurações são armazenadas no sistema de arquivos interno (LittleFS)
 * em formato JSON no arquivo:
 *
 * `/config.json`
 *
 * Este subsistema permite:
 *
 * - Salvar configurações
 * - Carregar configurações
 * - Verificar existência de configuração
 *
 * As configurações incluem:
 *
 * - Credenciais Wi-Fi
 * - Configurações MQTT
 * - Limites operacionais de sensores
 *
 * @note O sistema de arquivos LittleFS deve ser inicializado antes do uso.
 *
 * @warning Sempre verifique o campo `valid` após loadConfig().
 */

#ifndef CONFIG_STORAGE
#define CONFIG_STORAGE

#include <Arduino.h>

/**
 * @struct ConfigurationData
 * @brief Estrutura que contém todos os parâmetros configuráveis do sistema.
 *
 * @details
 * Esta estrutura representa a configuração completa do dispositivo.
 *
 * É carregada a partir do armazenamento persistente e utilizada
 * por todos os módulos do sistema.
 *
 * O campo `valid` indica se a configuração foi carregada corretamente.
 *
 * @note Se valid == false, a configuração não deve ser utilizada.
 */
struct ConfigurationData
{

  /**
   * @brief Endereço do broker MQTT.
   *
   * Exemplo:
   *
   * broker.hivemq.com
   */
  String mqttServer;

  /**
   * @brief Usuário MQTT.
   */
  String mqttUser;

  /**
   * @brief Senha MQTT.
   */
  String mqttPass;

  /**
   * @brief Tópico MQTT de publicação de dados.
   */
  String mqttTopicData;

  /**
   * @brief Identificador único do dispositivo.
   */
  String mqttTopicAlert;

  /**
   * @brief Identificador único do dispositivo.
   */
  String mqttDeviceId;

  /**
   * @brief Porta do servidor MQTT.
   *
   * Exemplo:
   *
   * 1883
   */
  int mqttPort;

  /**
   * @brief Porta do servidor MQTT.
   *
   * Exemplo:
   *
   * 1883
   */
  String wifiPass;

  /**
   * @brief Nome da rede Wi-Fi (SSID).
   */
  String wifiSSID;

  /**
   * @brief Temperatura máxima permitida.
   */
  float temperatureMax;

  /**
   * @brief Temperatura máxima permitida.
   */
  float temperatureMin;

  /**
   * @brief Umidade máxima permitida.
   */
  float humidityMax;

  /**
   * @brief Umidade mínima permitida.
   */
  float humidityMin; /**< Umidade mínima permitida (%) */

  /**
   * @brief Indica se os dados são válidos.
   *
   * true  → Configuração carregada corretamente
   *
   * false → Configuração inválida ou inexistente
   *
   * @warning Sempre verificar antes de usar a configuração.
   */
  bool valid = false; /**< Indica se os dados carregados são válidos */
};


// ============================================================================
// Funções
// ============================================================================


/**
 * @brief Inicializa o subsistema de configuração.
 *
 * @details
 * Prepara o módulo para operações de leitura e escrita.
 *
 * @note Não monta o LittleFS.
 */
void initConfigStorage();

/**
 * @brief Carrega as configurações persistentes.
 *
 * @details
 * Lê o arquivo `/config.json` e converte para ConfigurationData.
 *
 * @return Estrutura contendo a configuração carregada.
 *
 * @warning Verifique config.valid antes de usar.
 */
ConfigurationData loadConfig();

/**
 * @brief Salva configuração no armazenamento persistente.
 *
 * @param config Configuração a ser salva.
 *
 * @retval true Sucesso
 * @retval false Falha
 */
bool saveConfigutionData(const ConfigurationData &config);

/**
 * @brief Verifica se existe configuração salva.
 *
 * @retval true Existe
 * @retval false Não existe
 */
bool hasConfigData();

// ============================================================================
// Variáveis globais
// ============================================================================


/**
 * @brief Configuração global ativa do sistema.
 *
 * @details
 * Esta variável contém a configuração atualmente carregada.
 *
 * É atualizada automaticamente após:
 *
 * - loadConfig()
 * - saveConfigutionData()
 *
 * @warning Não usar antes de carregar configuração.
 */
extern ConfigurationData cfg;

#endif
