/**
 * @file display_LCD-2004_I2C.cpp
 * @brief Implementação do módulo de controle do display LCD 20x4 via I2C.
 *
 * @details
 * Este módulo é responsável por gerenciar a interface de exibição
 * do sistema utilizando um display LCD 20x4 conectado através de
 * um módulo I2C.
 *
 * Funcionalidades principais:
 * - Inicialização do display
 * - Exibição de mensagens do sistema
 * - Indicação de falhas de comunicação (WiFi / MQTT)
 * - Exibição do número da enfermaria
 * - Exibição da quantidade de chamados pendentes
 * - Exibição do endereço IP do dispositivo
 *
 * O módulo também mantém o controle da tela atual para evitar
 * atualizações desnecessárias do display, reduzindo flicker
 * e melhorando a experiência visual.
 */

#include <Wire.h>                  // Inclui a biblioteca padrão para comunicação I2C
#include <LiquidCrystal_I2C.h>     // Biblioteca que controla o display LCD via módulo I2C
#include "display_LCD-2004_I2C.h"  // Inclusão do arquivo de cabeçalho do módulo que contém as declarações de funções



/**
 * @brief Endereço I2C do módulo controlador do LCD.
 *
 * @details
 * O endereço padrão da maioria dos módulos LCD I2C baseados
 * no chip PCF8574 é 0x27, porém pode variar dependendo do
 * fabricante do módulo.
 */
const uint8_t I2C_ADDR = 0x27;


/**
 * @brief Número de colunas do display LCD.
 */
const uint8_t LCD_COLUMNS = 20;


/**
 * @brief Número de linhas do display LCD.
 */
const uint8_t LCD_LINES = 4;


/**
 * @brief Estado atual da tela exibida no display.
 *
 * @details
 * Utilizado para evitar redesenho desnecessário da tela,
 * atualizando apenas quando o estado muda.
 */
DisplayState currentScreen = SCREEN_NONE;

/**
 * @brief Instância do objeto responsável pelo controle do LCD.
 *
 * @details
 * A biblioteca LiquidCrystal_I2C gerencia a comunicação
 * com o display através do barramento I2C.
 */
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);


/**
 * @brief Limpa parcialmente uma linha do display.
 *
 * @details
 * Esta função substitui um número definido de caracteres
 * por espaços em branco, permitindo apagar apenas uma
 * parte específica da tela sem precisar limpar o display inteiro.
 *
 * @param column Coluna inicial onde a limpeza começa.
 * @param line Linha do display a ser limpa.
 * @param count Quantidade de colunas a serem apagadas.
 */
void clearLine(uint8_t column, uint8_t line, uint8_t count) {
  if (line >= LCD_LINES || column >= LCD_COLUMNS) return;  // valida linha e coluna
  // Posiciona o cursor na área escolhida
  lcd.setCursor(column, line);
  // Imprime espaços para apagar o conteúdo anterior
  for (uint8_t i = 1; i <= count; i++) {
    lcd.print(F(" "));
  }
}


/**
 * @brief Exibe os elementos fixos da interface principal.
 *
 * @details
 * Esta função desenha os textos estáticos da tela principal
 * do sistema, incluindo:
 *
 * - Título da aplicação
 * - Separador visual
 * - Campo para número da enfermaria
 * - Campo para quantidade de chamados
 */
void fixed_data() {
  lcd.clear();          // Limpa a tela
  lcd.setCursor(1, 0);  // Posiciona o cursor na coluna 1, linha 0
  lcd.print(F("Chamada Enfermagem"));

  lcd.setCursor(0, 1);  // Posiciona o cursor na coluna 0, linha 1
  lcd.print(F("--------------------"));

  lcd.setCursor(0, 2);  // Posiciona o cursor na coluna 0, linha 2
  lcd.print(F("Enfermaria: "));

  lcd.setCursor(0, 3);  // Posiciona o cursor na coluna 0, linha 3
  lcd.print(F("Quant. Chamados:"));
}


/**
 * @brief Exibe mensagem de falha no sistema.
 *
 * @details
 * Apresenta uma mensagem de erro indicando falha
 * na comunicação do sistema, podendo ser relacionada
 * à conexão WiFi ou ao broker MQTT.
 *
 * A função evita redesenhar a mesma tela de erro caso
 * ela já esteja sendo exibida.
 *
 * @param msgType Tipo de falha a ser exibida (WiFi ou MQTT).
 */
void showFailureMessage(const char* msgType) {
  // Validações dos tipos de Falhas
  if (msgType == MESSAGE_WIFI) {                 // Se for falha no wifi
    if (currentScreen == SCREEN_FAILURE_WIFI) {  // Verifica se já não mostrou a tela de erro wifi, se tiver mostrado retorna
      return;
    }
    // Caso não tenha mostrado a tela de erro wifi,
    //  currentScreen recebe a SCREEN_FAILURE_WIFI e prossegue com o código de mostrar a falha wifi na tela
    currentScreen = SCREEN_FAILURE_WIFI;
  }
  // Caso não seja falha wifi, é falha no MQTT
  else {
    // Verifica se já mostrou a tela de falha correspondente
    //  SE tiver mostrado, retorna
    if (currentScreen == SCREEN_FAILURE_MQTT) {
      return;
    }
    // Se tava em outra tela,
    //  currentScreen recebe a SCREEN_FAILURE_MQTT e prossegue com o código de mostrar a falha MQTT na tela
    currentScreen = SCREEN_FAILURE_MQTT;
  }

  lcd.clear();          // Limpa a área
  lcd.setCursor(6, 0);  // Posiciona o cursor na coluna 6, linha 0
  lcd.print(F("Falha!!!"));

  lcd.setCursor(0, 1);  // Posiciona o cursor na coluna 0, linha 1
  lcd.print(F("--------------------"));

  lcd.setCursor(4, 2);
  lcd.print(F("Confira "));
  lcd.print(msgType);

  lcd.setCursor(4, 3);
  lcd.print(F("< > = voltar"));
}


/**
 * @brief Exibe tela de confirmação para finalização de chamada.
 *
 * @details
 * Esta tela solicita confirmação do usuário antes de
 * remover um chamado da lista de pendências.
 *
 * @param infirmary Número da enfermaria selecionada.
 */
void showExclusionConfirm(const char* infirmary) {
  // Indica que está na tela de confirmação de exclusão
  currentScreen = SCREEN_EXCLUSION_CONFIRM;
  lcd.clear();  // Limpa a tela toda
  lcd.setCursor(1, 0);
  lcd.print(F("Finalizar Chamada"));

  lcd.setCursor(2, 1);
  lcd.print(F("Enfermaria "));
  lcd.print(infirmary);
  lcd.print(F("?"));

  lcd.setCursor(0, 2);
  lcd.print(F("DEL = confirmar"));

  lcd.setCursor(0, 3);
  lcd.print(F("< > = voltar"));
}


/**
 * @brief Exibe o número da enfermaria atual e quantidade de chamados.
 *
 * @details
 * Atualiza a tela principal mostrando:
 * - Número da enfermaria selecionada
 * - Quantidade de chamados pendentes
 *
 * Se existir mais de um chamado, são exibidas setas
 * indicando possibilidade de navegação entre itens.
 *
 * @param infirmary Número da enfermaria exibida.
 * @param hasNursingCall Indica se existe chamado ativo.
 * @param total_items Quantidade total de chamados pendentes.
 */
void showInfirmaryNumber(const char* infirmary, bool hasNursingCall, int total_items) {
  // Se não estiver na tela MAIN (esta tela)
  if (currentScreen != SCREEN_MAIN) {
    fixed_data();  // Mostra os dados Fixos
  } else {         // se já tiver nessa tela, limpa só a região necessária
    // Limpa 8 espaços na coluna/linha escolhida - ENFERMARIA
    clearLine(12, 2, 8);
    // Limpa 3 espaços na coluna/linha escolhida - QUANT. CHAMADAS
    clearLine(17, 3, 3);
  }

  currentScreen = SCREEN_MAIN;

  // Linha 3 - mostrando o número da enfermaria
  lcd.setCursor(12, 2);
  if (hasNursingCall) {
    // Se tiver mais de um chamado não resolvido, mostra "< num >"
    if (total_items > 1) {
      lcd.print(F("< "));
      lcd.print(infirmary);
      lcd.print(F(" >"));
    } else {
      lcd.print(infirmary);
    }

  } else {
    lcd.print(F(" X"));
  }

  // Linha 4 - mostrando a quantidade de chamados não resolvidos
  lcd.setCursor(17, 3);
  lcd.print(total_items);
}


/**
 * @brief Exibe o endereço IP obtido pelo dispositivo.
 *
 * @details
 * Mostra informações da conexão WiFi juntamente
 * com o endereço IP atual do dispositivo na rede.
 *
 * @param msg_IPAddress String contendo o endereço IP.
 */
void showIPAddress(const char* msg_IPAddress) {
  // Indica que está na tela de IP
  currentScreen = SCREEN_IPADDRESS;
  lcd.clear();          // Limpa a tela toda
  lcd.setCursor(2, 0);  // Posiciona o cursor na coluna 2, linha 0
  lcd.print(F("Wi-Fi Conectado"));

  lcd.setCursor(0, 1);  // Posiciona o cursor na coluna 0, linha 1
  lcd.print(F("--------------------"));

  lcd.setCursor(0, 2);  // Posiciona o cursor na coluna 0, linha 2
  lcd.print(F("IP: "));
  lcd.print(msg_IPAddress);

  lcd.setCursor(0, 3);
  lcd.print(F("< > = voltar"));
}


/**
 * @brief Inicializa o display LCD.
 *
 * @details
 * Realiza a inicialização do barramento I2C e
 * configura o display LCD para operação.
 *
 * Também ativa a iluminação de fundo do display.
 *
 * @note Deve ser chamada durante a inicialização
 * do sistema (normalmente no `setup()`).
 */
void lcd2004_init() {
  Wire.begin();     // para garantir que I2C foi inicializada
  lcd.init();       // Inicializa o display e a comunicação I2C
  lcd.backlight();  // Liga a luz de fundo do display
}
