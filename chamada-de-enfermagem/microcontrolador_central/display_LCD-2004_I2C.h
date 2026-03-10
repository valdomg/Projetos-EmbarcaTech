/**
 * @file display_LCD-2004_I2C.h
 * @brief Interface do módulo de controle do display LCD 20x4 via I2C.
 *
 * @details
 * Este módulo fornece funções responsáveis pela interface visual
 * do sistema utilizando um display LCD 20x4 conectado através de
 * um módulo I2C.
 *
 * O display é utilizado para apresentar informações operacionais
 * do sistema, incluindo:
 *
 * - Número da enfermaria selecionada
 * - Quantidade de chamados pendentes
 * - Mensagens de erro de comunicação (WiFi ou MQTT)
 * - Confirmação de ações do usuário
 * - Endereço IP do dispositivo na rede
 *
 * O módulo também implementa uma pequena máquina de estados para
 * controlar qual tela está sendo exibida no momento, evitando
 * atualizações desnecessárias do display.
 *
 * @note Este módulo depende das bibliotecas:
 * - Wire
 * - LiquidCrystal_I2C
 *
 * @date 2026
 */

#ifndef DISPLAY_LCD_2004_I2C_H // Verifica se o cabeçalho já foi incluído
#define DISPLAY_LCD_2004_I2C_H // Define o identificador do cabeçalho para evitar inclusões múltiplas


/**
 * @brief Identificador de falha de conexão WiFi.
 *
 * Utilizado pela função `showFailureMessage()` para indicar
 * que ocorreu erro na conexão com a rede WiFi.
 */
constexpr const char* MESSAGE_WIFI = "Wi-Fi";

/**
 * @brief Identificador de falha de comunicação MQTT.
 *
 * Utilizado pela função `showFailureMessage()` para indicar
 * que ocorreu erro na comunicação com o broker MQTT.
 */
constexpr const char* MESSAGE_MQTT = "MQTT";


// -----------------------------------------------------------------------------
// Máquina de estados do display
// -----------------------------------------------------------------------------

/**
 * @enum DisplayState
 * @brief Define os estados possíveis das telas exibidas no display.
 *
 * @details
 * Essa enumeração representa a máquina de estados responsável
 * por controlar qual tela está atualmente ativa no display.
 *
 * O uso dessa estrutura permite evitar redesenhos desnecessários
 * da tela e facilita o gerenciamento da interface do usuário.
 */
enum DisplayState {

    /** Nenhuma tela ativa */
    SCREEN_NONE,

    /** Tela principal com número da enfermaria e quantidade de chamados */
    SCREEN_MAIN,

    /**
     * Tela de falha na conexão WiFi.
     *
     * Esta tela é exibida apenas uma vez enquanto a falha persistir.
     */
    SCREEN_FAILURE_WIFI,

    /**
     * Tela de falha na comunicação MQTT.
     *
     * Esta tela também é exibida apenas uma vez enquanto o erro persistir.
     */
    SCREEN_FAILURE_MQTT,

    /** Tela de confirmação para finalizar um chamado */
    SCREEN_EXCLUSION_CONFIRM,

    /** Tela que exibe o endereço IP do dispositivo */
    SCREEN_IPADDRESS
};

/**
 * @brief Estado atual da interface do display.
 *
 * @details
 * Essa variável mantém o estado da tela atualmente exibida.
 * Ela é utilizada para controlar a máquina de estados da
 * interface gráfica e evitar atualizações desnecessárias
 * no display.
 */
extern DisplayState currentScreen;

// -----------------------------------------------------------------------------
// Funções públicas do módulo
// -----------------------------------------------------------------------------

/**
 * @brief Inicializa o display LCD 20x4.
 *
 * @details
 * Esta função inicializa a comunicação I2C e prepara o display
 * para operação.
 *
 * Durante a inicialização:
 * - O barramento I2C é iniciado
 * - O display é configurado
 * - A iluminação de fundo é ativada
 *
 * @note Deve ser chamada durante o `setup()` do sistema.
 */
extern void lcd2004_init();

/**
 * @brief Exibe mensagem de falha no display.
 *
 * @details
 * Apresenta uma mensagem informando falha de comunicação
 * no sistema.
 *
 * Os tipos de falha suportados são:
 *
 * - `MESSAGE_WIFI`
 * - `MESSAGE_MQTT`
 *
 * @param msgType Tipo da falha a ser exibida.
 */
extern void showFailureMessage(const char* msgType);

/**
 * @brief Exibe a tela principal com dados da enfermaria.
 *
 * @details
 * Mostra no display:
 *
 * - Número da enfermaria selecionada
 * - Quantidade de chamados pendentes
 *
 * Caso existam múltiplos chamados, setas de navegação
 * são exibidas ao lado do número da enfermaria.
 *
 * @param infirmary Número da enfermaria.
 * @param hasNursingCall Indica se existe chamado ativo.
 * @param total_items Quantidade total de chamados pendentes.
 */
extern void showInfirmaryNumber(const char* infirmary, bool hasNursingCall, int total_items);

/**
 * @brief Exibe tela de confirmação para finalização de chamado.
 *
 * @details
 * Solicita confirmação do usuário antes de remover
 * um chamado da lista de pendências.
 *
 * @param infirmary Número da enfermaria selecionada.
 */
extern void showExclusionConfirm(const char* infirmary);

/**
 * @brief Exibe o endereço IP do dispositivo.
 *
 * @details
 * Mostra informações da conexão WiFi juntamente
 * com o endereço IP obtido pelo dispositivo na rede.
 *
 * @param msg_IPAddress Endereço IP do dispositivo.
 */
extern void showIPAddress(const char* msg_IPAddress);


#endif // Final da diretiva de proteção contra múltiplas inclusões