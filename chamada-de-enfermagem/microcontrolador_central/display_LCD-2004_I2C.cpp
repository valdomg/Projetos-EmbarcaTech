#include <Wire.h>                  // Inclui a biblioteca padrão para comunicação I2C
#include <LiquidCrystal_I2C.h>     // Biblioteca que controla o display LCD via módulo I2C
#include "display_LCD-2004_I2C.h"  // Inclusão do arquivo de cabeçalho do módulo que contém as declarações de funções



const uint8_t I2C_ADDR = 0x27;  // Endereço padrão do módulo I2C do LCD
// Define quantidade de linhas e colunas do LCD (20 colunas e 4 linhas)
const uint8_t LCD_COLUMNS = 20;
const uint8_t LCD_LINES = 4;


// Instancia do LCD
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);


void fixed_data() {
  lcd.setCursor(1, 0);  // Posiciona o cursor na coluna 1, linha 0
  lcd.print(F("Chamada Enfermagem"));

  lcd.setCursor(0, 1);  // Posiciona o cursor na coluna 0, linha 1
  lcd.print(F("--------------------"));

  lcd.setCursor(0, 2);  // Posiciona o cursor na coluna 0, linha 2
  lcd.print(F("Enfermaria: "));

  lcd.setCursor(0, 3);  // Posiciona o cursor na coluna 0, linha 3
  lcd.print(F("Quant. Chamados:"));
}


// Função que inicializa o Display
void lcd2004_init() {
  Wire.begin();     // para garantir que I2C foi inicializada
  lcd.init();       // Inicializa o display e a comunicação I2C
  lcd.backlight();  // Liga a luz de fundo do display

  // Mostrar os dados fixos
  fixed_data();
}
