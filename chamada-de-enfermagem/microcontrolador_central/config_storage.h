#ifndef CONFIG_STORAGE
#define CONFIG_STORAGE

#include <Arduino.h>

/**
 * @file config_storage.h
 * @brief Declarações relacionadas ao armazenamento e recuperação de configurações persistentes.
 * 
 * Este módulo define a estrutura de dados que armazena as configurações do sistema
 * (como credenciais de Wi-Fi, parâmetros MQTT e limites ambientais) e fornece funções
 * para salvar e carregar essas informações em memória persistente utilizando o sistema de arquivos LittleFS.
 */

/**
 * @struct ConfigurationData
 * @brief Estrutura que contém os parâmetros de configuração do sistema.
 * 
 * Esta estrutura armazena todas as informações necessárias para o funcionamento
 * do sistema de monitoramento e comunicação MQTT, incluindo credenciais de rede
 * e limites de operação dos sensores.
 */
struct ConfigurationData {
  String mqttServer;        /**< Endereço do servidor MQTT (ex.: "broker.hivemq.com") */
  String mqttUser;          /**< Usuário para autenticação no servidor MQTT */
  String mqttPass;          /**< Senha para autenticação no servidor MQTT */
  String mqttDeviceId;      /**< Identificador único do dispositivo (Device ID) */
  int mqttPort;      /**< Identificador único do dispositivo (Device ID) */

  String wifiPass;          /**< Senha da rede Wi-Fi */
  String wifiSSID;          /**< SSID (nome) da rede Wi-Fi */

  bool valid = false;       /**< Indica se os dados carregados são válidos */
};

/**
 * @brief Inicializa o módulo de armazenamento de configuração.
 * 
 * Responsável por preparar o ambiente de leitura e escrita no sistema de arquivos
 * (LittleFS). Caso o sistema já tenha sido montado em outro módulo, esta função
 * mantém a consistência sem duplicar inicializações.
 */
void initConfigStorage();

/**
 * @brief Carrega as configurações salvas no arquivo `/config.json`.
 * 
 * Lê o conteúdo do arquivo de configuração armazenado no sistema de arquivos
 * e converte-o em uma estrutura `ConfigurationData`.
 * 
 * @return Estrutura `ConfigurationData` contendo os parâmetros carregados.
 *         Se o arquivo não existir ou houver erro de leitura, retorna estrutura com `valid = false`.
 */
ConfigurationData loadConfig();

/**
 * @brief Salva as configurações atuais no arquivo `/config.json`.
 * 
 * Converte a estrutura `ConfigurationData` para o formato JSON e a armazena
 * de forma persistente no sistema de arquivos LittleFS.
 * 
 * @param config Estrutura contendo os valores de configuração a serem salvos.
 * @return true se o salvamento for bem-sucedido, false caso contrário.
 */
bool saveConfigutionData(const ConfigurationData &config);

/**
 * @brief Verifica se existe um arquivo de configuração armazenado.
 * 
 * @return true se o arquivo `/config.json` existir.
 * @return false se o arquivo não for encontrado.
 */
bool hasConfigData();

/**
 * @brief Variável global que contém os dados de configuração ativos.
 * 
 * Essa variável é atualizada ao carregar ou salvar os dados e
 * pode ser utilizada em outros módulos para acessar os parâmetros
 * configurados do sistema.
 */
extern ConfigurationData cfg;

#endif
