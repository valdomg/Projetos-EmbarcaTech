#include <Wire.h>                  // Inclui a biblioteca padrão para comunicação I2C
#include <LiquidCrystal_I2C.h>     // Biblioteca que controla o display LCD via módulo I2C
#include "display_LCD-2004_I2C.h"  // Inclusão do arquivo de cabeçalho do módulo que contém as declarações de funções


const uint8_t I2C_ADDR = 0x27;  // Endereço padrão do módulo I2C do LCD
// Define quantidade de linhas e colunas do LCD (20 colunas e 4 linhas)
const uint8_t LCD_COLUMNS = 20;
const uint8_t LCD_LINES = 4;


// Instancia do LCD
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);


/*Função de Limpeza de Linha
    Parâmetro:
      - column: coluna do display a ser limpa (uint8_t)
      - line: linha do display a ser limpa (uint8_t) 
      - count: Quant. colunas a ser limpas (uint8_t)*/
void clearLine(uint8_t column, uint8_t line, uint8_t count) {
  if (line >= LCD_LINES || column >= LCD_COLUMNS) return;  // valida linha e coluna
  // Posiciona o cursor na área escolhida
  lcd.setCursor(column, line);
  // Imprime espaços para apagar o conteúdo anterior
  for (uint8_t i = 1; i <= count; i++) {
    lcd.print(F(" "));
  }
}


// Função que mostra os dados no display
void show_infirmary_numberCalls(List_NursingCall &listCalls) {
  // Limpa 8 espaços na coluna/linha escolhida - ENFERMARIA
  clearLine(12, 2, 8);

  // Limpa 3 espaços na coluna/linha escolhida - QUANT. CHAMADAS
  clearLine(17, 3, 3);


  // Linha 3 - mostrando o número da enfermaria
  lcd.setCursor(12, 2);
  if (listCalls.hasNursingCall()) {
    lcd.print(F("< "));
    lcd.print(listCalls.getInfirmaryCurrent());
    lcd.print(F(" >"));

  } else {
    lcd.print(F(" X"));
  }

  // Linha 4 - mostrando a quantidade de chamados não resolvidos
  lcd.setCursor(17, 3);
  lcd.print(listCalls.getTotal());
}


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
