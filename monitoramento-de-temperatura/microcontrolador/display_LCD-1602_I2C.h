/**
 * @file display_lcd_1602_i2c.h
 * @brief Interface para controle do display LCD 1602 com módulo I2C.
 *
 * Este arquivo contém as declarações das funções responsáveis pela
 * inicialização, exibição de dados e controle da luz de fundo
 * do display LCD 1602 conectado via interface I2C.
 *
 * O display é utilizado para apresentar informações de temperatura,
 * umidade e estados de alerta do sistema.
 */

#ifndef DISPLAY_LCD_1602_I2C_H
#define DISPLAY_LCD_1602_I2C_H


/**
 * @brief Inicializa o display LCD 1602 via I2C.
 *
 * Esta função deve ser chamada durante a inicialização do sistema
 * (por exemplo, dentro da função setup()).
 *
 * Ela realiza a configuração inicial do display, incluindo
 * comunicação I2C, limpeza da tela e preparação para exibição.
 *
 * @note Deve ser chamada antes de qualquer outra função do display.
 */
extern void lcd1602_init();


/**
 * @brief Exibe os valores de temperatura e umidade no display LCD.
 *
 * Esta função atualiza o conteúdo do display com os valores atuais
 * de temperatura e umidade, além de indicar visualmente estados
 * de alerta quando os limites definidos forem ultrapassados.
 *
 * @param temp Valor da temperatura em graus Celsius.
 * @param humi Valor da umidade relativa do ar em porcentagem.
 * @param alertTemp Indica se existe alerta de temperatura.
 * - true: temperatura em estado de alerta
 * - false: temperatura em estado normal
 * @param alertHumi Indica se existe alerta de umidade.
 * - true: umidade em estado de alerta
 * - false: umidade em estado normal
 *
 * @note O formato de exibição depende da implementação no arquivo .cpp.
 */
extern void lcd1602_showData(float temp, float humi, bool alertTemp, bool alertHumi);


/**
 * @brief Controla o estado da luz de fundo do display LCD.
 *
 * Esta função gerencia o backlight do display com base no estado
 * de alerta do sistema ou em um comando manual.
 *
 * @param isAlertState Indica se o sistema está em estado de alerta.
 * - true: ativa comportamento de alerta
 * - false: operação normal
 *
 * @param commandTurnOnLCD Comando manual para ligar o display.
 * - true: liga o backlight
 * - false: desliga o backlight
 *
 * @note Pode ser usado para economizar energia ou sinalizar alertas visuais.
 */
extern void handleBacklightLCD(bool isAlertState, bool commandTurnOnLCD);


#endif