/**
 * @file display_LCD-1602_I2C.cpp
 * @brief Implementação do controle do display LCD 1602 via I2C.
 *
 * Inclui a biblioteca padrão para comunicação I2C,
 * a biblioteca de controle do display LCD 1602,
 * e funções matemáticas utilizadas para cálculo de valor absoluto.
 *
 * Este módulo implementa funções para:
 *
 * - Inicializar o display
 * - Limpar linhas
 * - Exibir temperatura e umidade
 * - Controlar o backlight
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>

#include "display_LCD-1602_I2C.h"
#include "log.h"


/**
 * @brief Endereço padrão do módulo I2C do LCD.
 */
const uint8_t I2C_ADDR = 0x27;


/**
 * @brief Quantidade de colunas do LCD.
 */
const uint8_t LCD_COLUMNS = 16;


/**
 * @brief Quantidade de linhas do LCD.
 */
const uint8_t LCD_LINES = 2;



/**
 * @brief Mensagem exibida quando temperatura ultrapassa o limite.
 */
#define TEMPERATURE_ALERT_MESSAGE F("ALERT_T: ")


/**
 * @brief Mensagem exibida quando temperatura está dentro do limite.
 */
#define TEMPERATURE_MESSAGE F("T: ")


/**
 * @brief Mensagem exibida quando umidade ultrapassa o limite.
 */
#define HUMIDITY_ALERT_MESSAGE F("ALERT_U: ")


/**
 * @brief Mensagem exibida quando umidade está dentro do limite.
 */
#define HUMIDITY_MESSAGE F("U: ")



/**
 * @brief Último valor de temperatura exibido.
 */
float lastTemperature = -1000.0;


/**
 * @brief Último valor de umidade exibido.
 */
float lastHumidity = -1000.0;



/**
 * @brief Limiar mínimo para atualização da temperatura.
 *
 * Evita reescrever o display sem necessidade.
 */
const float TEMP_UPDATE_THRESHOLD = 0.01;


/**
 * @brief Limiar mínimo para atualização da umidade.
 */
const float HUMI_UPDATE_THRESHOLD = 0.01;



/**
 * @brief Indica se o backlight está ligado.
 */
bool isBacklightTurnOn = true;


/**
 * @brief Guarda o tempo da última ativação do backlight.
 */
unsigned long lastTimeBackligthTurnOn = 0;



/**
 * @brief Instância do LCD.
 */
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);



// ------------------------------------------------------------
// Funções
// ------------------------------------------------------------


/**
 * @brief Inicializa o display.
 *
 * Inicializa comunicação I2C,
 * ativa o backlight
 * e exibe mensagem inicial.
 */
void lcd1602_init() {
  Wire.begin();         // para garantir que I2C foi inicializada
  lcd.init();           // Inicializa o display e a comunicação I2C
  lcd.backlight();      // Liga a luz de fundo do display
  lcd.setCursor(0, 0);  // Posiciona o cursor na coluna 0, linha 0
  // Usa F(" ") para economizar RAM ao imprimir
  lcd.print(F("Iniciando..."));  // Escreve a mensagem inicial no display
  delay(300);                    // Aguarda 300 ms
  lcd.clear();                   // Limpa completamente o display
}


/**
 * @brief Limpa uma linha do display.
 *
 * @param line Linha do display a ser limpa (uint8_t).
 */
void clearLine(uint8_t line) {
  if (line >= LCD_LINES) return;  // valida linha
  // Posiciona o cursor no início da linha indicada
  lcd.setCursor(0, line);
  // Imprime 16 espaços (uma linha cheia) para apagar o conteúdo anterior
  for (int i = 0; i < LCD_COLUMNS; i++) {
    lcd.print(F(" "));
  }
}


/**
 * @brief Exibe no display o valor de temperatura ou umidade.
 *
 * Função genérica responsável por mostrar uma mensagem e o valor
 * correspondente em uma linha específica do display, podendo
 * alternar entre mensagem normal e mensagem de alerta.
 *
 * @param line Linha do display (0 = primeira linha, 1 = segunda linha).
 * @param normalMsg Ponteiro para mensagem normal armazenada na memória Flash.
 * @param alertMsg Ponteiro para mensagem de alerta armazenada na memória Flash.
 * @param value Valor de temperatura ou umidade a ser exibido.
 * @param alert Define se deve exibir a mensagem de alerta (true) ou normal (false).
 * @param unit Sufixo da unidade de medida (ex.: "%" ou "\xDF""C").
 */
void show_value(uint8_t line,
                const __FlashStringHelper* normalMsg,
                const __FlashStringHelper* alertMsg,
                float value,
                bool alert,
                const char* unit) {
  // Limpa a linha antes de escrever
  clearLine(line);
  // Posição inicial da linha
  lcd.setCursor(0, line);
  // escolhe e imprime a mensagem apropriada diretamente da Flash
  //    Se alert for verdadeiro, exibe a mensagem de alerta. Caso contrário, mostra só a mensagem normal "U: " ou "T: "
  lcd.print(alert ? alertMsg : normalMsg);
  // Imprime o valor no display com 2 casa decimal
  lcd.print(value, 2);
  // Imprime a Unidade ("°C" ou "%")
  lcd.print(unit);
}


/**
 * @brief Exibe os valores de temperatura e umidade no display LCD.
 *
 * Esta função atualiza o display com os valores atuais de temperatura
 * e umidade, podendo indicar visualmente situações de alerta quando
 * os limites configurados são ultrapassados.
 *
 * @param temp Valor da temperatura em graus Celsius.
 * @param humi Valor da umidade relativa do ar em porcentagem.
 * @param alertTemp Indica condição de alerta para temperatura.
 * @param alertHumi Indica condição de alerta para umidade.
 */
void lcd1602_showData(float temp, float humi, bool alertTemp, bool alertHumi) {

  // Só chama a função se a temperatura mudou além do limiar (>= 0,01)
  if (fabs(temp - lastTemperature) >= TEMP_UPDATE_THRESHOLD) {
    // Linha 0: temperatura com prefixo "T: " ou "ALERT_T: ", sufixo "°C".
    show_value(0,
               TEMPERATURE_MESSAGE,
               TEMPERATURE_ALERT_MESSAGE,
               temp,
               alertTemp,
               "\xDF"
               "C");
    lastTemperature = temp;  // atualiza último valor
  }

  // Só chama a função se a umidade mudou além do limiar
  if (fabs(humi - lastHumidity) >= HUMI_UPDATE_THRESHOLD) {
    // Linha 1: umidade com prefixo "U: " ou "ALERT_U: ", sufixo "%".
    show_value(1,
               HUMIDITY_MESSAGE,
               HUMIDITY_ALERT_MESSAGE,
               humi,
               alertHumi,
               "%");
    lastHumidity = humi;  // atualiza último valor
  }
}

/**
 * @brief Liga o backlight.
 */
void turnOnBacklight() {
  if (isBacklightTurnOn == false) {
    lcd.backlight();
    isBacklightTurnOn = true;
  }
}

/**
 * @brief Desliga o backlight.
 */
void turnOffBacklight() {
  if (isBacklightTurnOn == true) {
    lcd.noBacklight();
    isBacklightTurnOn = false;
  }
}

/**
 * @brief Controla o estado do backlight.
 *
 * @param isAlertState Indica alerta.
 * @param commandTurnOnLCD Comando manual.
 */
void handleBacklightLCD(bool isAlertState, bool commandTurnOnLCD) {

  if (isAlertState) {
    turnOnBacklight();
  } else {

    unsigned long now = millis();
    if (commandTurnOnLCD) {
      turnOnBacklight();
      lastTimeBackligthTurnOn = now;

    } else {
      // log(LOG_INFO,"começou a contagem");
      if (now - lastTimeBackligthTurnOn > 10 * 1000) {
        turnOffBacklight();
      }
    }

  }
}
