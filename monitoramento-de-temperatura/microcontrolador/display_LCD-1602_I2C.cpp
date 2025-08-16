#include <Wire.h> // Inclui a biblioteca padrão para comunicação I2C
#include <LiquidCrystal_I2C.h> // Biblioteca que controla o display LCD 1602 via módulo I2C
#include "display_LCD-1602_I2C.h" // Inclusão do arquivo de cabeçalho do módulo que contém as declarações de funções


const uint8_t I2C_ADDR = 0x27; // Endereço padrão do módulo I2C do LCD
// Define quantidade de linhas e colunas do LCD (16 colunas e 2 linhas)
const uint8_t LCD_COLUMNS = 16;
const uint8_t LCD_LINES = 2;

// Mensagem padrão exibida no display quando a temperatura ultrapassa o limite
const char* TEMPERATURE_ALERT_MESSAGE = "ALERT TEMP ";
// Mensagem padrão usada para exibir a temperatura quando está dentro do limite
const char* TEMPERATURE_MESSAGE = "Temp: ";
// Mensagem padrão exibida no display quando a umidade ultrapassa o limite
const char* HUMIDITY_ALERT_MESSAGE = "ALERT UMI ";
// Mensagem padrão usada para exibir a umidade quando está dentro do limite
const char* HUMIDITY_MESSAGE = "Umid: ";

// Código ASCII do símbolo de grau
const uint8_t DEGREE_SYMBOL = 223;


// Instancia do LCD
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);


// Função que inicializa o Display
void initializeDisplayLCD_1602_I2C() {
  lcd.init(); // Inicializa o display e a comunicação I2C
  lcd.backlight(); // Liga a luz de fundo do display
  lcd.setCursor(0, 0); // Posiciona o cursor na coluna 0, linha 0
  lcd.print(F("Iniciando...")); // Escreve a mensagem inicial no display
  delay(300); // Aguarda 300 ms
  lcd.clear(); // Limpa completamente o display
}


/*Função de Limpeza de Linha
    Parâmetro:
      - line: linha do display a ser limpa (uint8_t) */
void clearLine(uint8_t line) {
  // Posiciona o cursor no início da linha indicada
  lcd.setCursor(0, line);
  // Imprime 16 espaços (uma linha cheia) para apagar o conteúdo anterior
  for (int i = 0; i < LCD_COLUMNS; i++) {
    lcd.print(" ");
  }
}

// Função genérica substitui show_temp e show_humi
void show_value(uint8_t line,
                const char* normalMsg,
                const char* alertMsg,
                float value,
                bool alert,
                const char* unit) {
  clearLine(line);
  lcd.setCursor(0, line);
  lcd.print(alert ? alertMsg : normalMsg);
  lcd.print(value, 2);
  lcd.print(unit);
}


/* Função que exibe os valores de temperatura e umidade no display
   Parâmetros:
     - temp: valor da temperatura (float)
     - humi: valor da umidade (float)
     - alertTemp: indica se a temperatura ultrapassou o limite (true = alerta)
     - alertHumi: indica se a umidade ultrapassou o limite (true = alerta)*/
void show_data_tempHum_DisplayLCD_1602_I2C(float temp, float humi, bool alertTemp, bool alertHumi) {
  // Exibe os valores de temperatura e umidade, passando os respectivos estados de alerta
  show_value(0, TEMPERATURE_MESSAGE, TEMPERATURE_ALERT_MESSAGE, temp, alertTemp, "C");
  show_value(1, HUMIDITY_MESSAGE, HUMIDITY_ALERT_MESSAGE, humi, alertHumi, "%");
}