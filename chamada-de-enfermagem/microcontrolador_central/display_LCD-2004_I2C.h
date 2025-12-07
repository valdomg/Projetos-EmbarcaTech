#ifndef DISPLAY_LCD_2004_I2C_H // Verifica se o cabeçalho já foi incluído
#define DISPLAY_LCD_2004_I2C_H // Define o identificador do cabeçalho para evitar inclusões múltiplas


// Declara a função que inicializa o  LCD 2004 via interface I2C
// Deve ser chamada no setup() para configurar o didisplaysplay
extern void lcd2004_init();
// Para as mensagens fixas
extern void fixed_data();
// Mostra dados já preparados
extern void showInfirmaryNumber(const char* infirmary, bool hasNursingCall, int total_items);
extern void showExclusionConfirm(const char* infirmary);


#endif // Final da diretiva de proteção contra múltiplas inclusões