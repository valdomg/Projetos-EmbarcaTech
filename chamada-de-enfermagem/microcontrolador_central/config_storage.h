/**
 * @file config_storage.h
 * @brief Interface do módulo de armazenamento persistente de configuração.
 *
 * @details
 * Este módulo define a estrutura de dados utilizada para armazenar
 * as configurações do sistema e fornece funções responsáveis por
 * salvar e carregar essas informações utilizando o sistema de
 * arquivos interno LittleFS.
 *
 * As configurações são armazenadas em formato JSON no arquivo
 * `/config.json`, permitindo persistência mesmo após reinicializações
 * do dispositivo.
 *
 * Entre os dados armazenados estão:
 *
 * - Credenciais da rede WiFi
 * - Parâmetros de conexão com o servidor MQTT
 * - Identificação do dispositivo no sistema
 *
 * O módulo fornece funções para:
 *
 * - Inicializar o subsistema de armazenamento
 * - Verificar se existe configuração salva
 * - Carregar configurações do armazenamento
 * - Salvar novas configurações
 *
 * @note Este módulo depende do sistema de arquivos LittleFS.
 *
 * @date 2026
 */

#ifndef CONFIG_STORAGE
#define CONFIG_STORAGE

#include <Arduino.h>



/**
 * @struct ConfigurationData
 * @brief Estrutura que armazena os parâmetros de configuração do sistema.
 *
 * @details
 * Esta estrutura contém todas as informações necessárias para que
 * o dispositivo consiga se conectar à rede WiFi e ao broker MQTT.
 *
 * Os dados são normalmente carregados do arquivo `/config.json`
 * durante a inicialização do sistema.
 *
 * O campo `valid` indica se os dados foram carregados corretamente.
 */
struct ConfigurationData {

  /** Endereço do servidor MQTT (ex.: "broker.hivemq.com") */
  String mqttServer;

  /** Usuário utilizado para autenticação no broker MQTT */
  String mqttUser;

  /** Senha utilizada para autenticação no broker MQTT */
  String mqttPass;

  /** Identificador único do dispositivo no sistema MQTT */
  String mqttDeviceId;

  /** Porta de conexão do servidor MQTT (ex.: 1883 ou 8883) */
  int mqttPort;

  /** Senha da rede WiFi */
  String wifiPass;

  /** Nome da rede WiFi (SSID) */
  String wifiSSID;

  /**
   * Indica se os dados carregados são válidos.
   *
   * Este campo é utilizado para verificar se a configuração
   * foi carregada corretamente do armazenamento persistente.
   */
  bool valid = false;
};


/**
 * @brief Inicializa o módulo de armazenamento de configuração.
 *
 * @details
 * Prepara o subsistema responsável por gerenciar as configurações
 * persistentes do dispositivo.
 *
 * Esta função normalmente realiza a inicialização do sistema
 * de arquivos LittleFS, permitindo leitura e escrita no
 * arquivo de configuração.
 *
 * @note Deve ser chamada durante a inicialização do sistema
 * (geralmente dentro do `setup()`).
 */
void initConfigStorage();


/**
 * @brief Carrega os dados de configuração armazenados.
 *
 * @details
 * Lê o arquivo `/config.json` presente no sistema de arquivos
 * LittleFS e converte seu conteúdo para uma estrutura
 * `ConfigurationData`.
 *
 * Caso o arquivo não exista ou ocorra erro durante a leitura,
 * será retornada uma estrutura com `valid = false`.
 *
 * @return Estrutura `ConfigurationData` contendo os dados carregados.
 */
ConfigurationData loadConfig();


/**
 * @brief Salva os dados de configuração no armazenamento persistente.
 *
 * @details
 * Converte os dados da estrutura `ConfigurationData`
 * para formato JSON e grava no arquivo `/config.json`
 * dentro do sistema de arquivos LittleFS.
 *
 * Caso o arquivo já exista, ele será sobrescrito.
 *
 * @param config Estrutura contendo os dados de configuração.
 *
 * @return true  Se os dados foram salvos corretamente.
 * @return false Caso ocorra erro ao gravar o arquivo.
 */
bool saveConfigutionData(const ConfigurationData &config);


/**
 * @brief Verifica se existe um arquivo de configuração salvo.
 *
 * @details
 * Confere se o arquivo `/config.json` está presente
 * no sistema de arquivos LittleFS.
 *
 * @return true  Se o arquivo existir.
 * @return false Caso contrário.
 */
bool hasConfigData();


/**
 * @brief Estrutura global contendo a configuração atual do sistema.
 *
 * @details
 * Esta variável armazena os dados de configuração atualmente
 * utilizados pelo sistema.
 *
 * Ela é atualizada quando:
 *
 * - O sistema carrega a configuração do armazenamento
 * - Uma nova configuração é salva
 *
 * Outros módulos podem acessar essa variável para obter
 * os parâmetros de configuração ativos.
 */
extern ConfigurationData cfg;


#endif