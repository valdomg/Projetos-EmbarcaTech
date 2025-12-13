#ifndef DISPLAY_LCD_2004_I2C_H // Verifica se o cabeçalho já foi incluído
#define DISPLAY_LCD_2004_I2C_H // Define o identificador do cabeçalho para evitar inclusões múltiplas


// Mensagens de erro padrão
constexpr const char* MESSAGE_WIFI = "Wi-Fi";
constexpr const char* MESSAGE_MQTT = "MQTT";


// Máquina de estados das telas
enum DisplayState {
    SCREEN_NONE,                // Nenhuma Tela
    SCREEN_MAIN,                // Tela: showInfirmaryNumber()
    SCREEN_FAILURE,             // Tela: showFailureMessage()
    SCREEN_EXCLUSION_CONFIRM,   // Tela: showExclusionConfirm()
};
extern DisplayState currentScreen;


// Declara a função que inicializa o  LCD 2004 via interface I2C
// Deve ser chamada no setup() para configurar o didisplaysplay
extern void lcd2004_init();
// Mostra a mensagem de falha
extern void showFailureMessage(const char* msgType);
// Mostra dados já preparados
extern void showInfirmaryNumber(const char* infirmary, bool hasNursingCall, int total_items);
extern void showExclusionConfirm(const char* infirmary);


#endif // Final da diretiva de proteção contra múltiplas inclusões