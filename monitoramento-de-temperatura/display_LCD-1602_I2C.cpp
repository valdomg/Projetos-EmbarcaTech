#include <Wire.h> // Inclui a biblioteca padrão para comunicação I2C
#include <LiquidCrystal_I2C.h> // Biblioteca que controla o display LCD 1602 via módulo I2C
#include "display_LCD-1602_I2C.h" // Inclusão do arquivo de cabeçalho do módulo que contém as declarações de funções


const uint8_t I2C_ADDR = 0x27; // Endereço padrão do módulo I2C do LCD
// Define quantidade de linhas e colunas do LCD (16 colunas e 2 linhas)
const uint8_t LCD_COLUMNS = 16;
const uint8_t LCD_LINES = 2;


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
