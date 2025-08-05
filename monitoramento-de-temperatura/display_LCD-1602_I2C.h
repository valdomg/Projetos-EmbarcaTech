#ifndef DISPLAY_LCD_1602_I2C_H // Verifica se o cabeçalho já foi incluído
#define DISPLAY_LCD_1602_I2C_H // Define o identificador do cabeçalho para evitar inclusões múltiplas


// Declara a função que inicializa o display LCD 1602 via interface I2C
// Deve ser chamada no setup() para configurar o display
extern void initializeDisplayLCD_1602_I2C();


#endif // Final da diretiva de proteção contra múltiplas inclusões