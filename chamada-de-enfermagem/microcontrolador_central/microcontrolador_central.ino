/**
 * @file nursing_station_firmware.ino
 * @brief Firmware principal do sistema de monitoramento de chamadas de enfermaria.
 *
 * @details
 * Este arquivo contém o ponto de entrada do firmware executado no
 * microcontrolador (ESP8266). Ele integra todos os módulos do sistema
 * responsáveis por comunicação, interface com o usuário e controle
 * de hardware.
 *
 * O sistema permite receber chamadas de enfermaria via MQTT,
 * armazená-las em uma lista duplamente encadeada e permitir navegação
 * e gerenciamento dessas chamadas através de botões físicos e de
 * um display LCD 20x4.
 *
 * Principais funcionalidades:
 * - Conexão com rede WiFi
 * - Comunicação com broker MQTT
 * - Recepção e processamento de mensagens JSON
 * - Armazenamento das chamadas em lista encadeada
 * - Navegação entre chamadas via botões
 * - Exibição das chamadas em display LCD
 * - Sinalização visual (LED) e sonora (buzzer)
 * - Modo de configuração via Access Point e servidor web
 *
 * Arquitetura do sistema:
 *
 * MQTT Broker
 *      │
 *      ▼
 * JSON Processing
 *      │
 *      ▼
 * Lista de Chamadas (List_NursingCall)
 *      │
 *      ├── Display LCD (interface do usuário)
 *      ├── LED indicador
 *      └── Buzzer de notificação
 *
 * Fluxo principal:
 * - setup() → inicializa hardware, rede e comunicação MQTT
 * - loop()  → executa continuamente a lógica do sistema
 *
 * @date 2026
 */

#include "wifi_utils.h"
#include "mqtt.h"
#include "server.h"
#include "display_LCD-2004_I2C.h"
#include "listNursingCall_utils.h"
#include "jsonDataProcessing.h"
#include "buttons.h"
#include "config.h"
#include "buzzer.h"
#include "led.h"
#include "config_storage.h"

// -----------------------------------------------------------------------------
// Variáveis globais
// -----------------------------------------------------------------------------

/**
 * @brief Indica se o botão de exclusão foi pressionado uma vez e
 * está aguardando confirmação do usuário.
 *
 * O sistema utiliza dois cliques no botão DELETE:
 * - Primeiro clique → exibe tela de confirmação
 * - Segundo clique → executa a exclusão da chamada
 */
bool deletionConfirmation = false;

/**
 * @brief Servidor HTTP utilizado no modo de configuração.
 *
 * Quando o dispositivo entra em modo de configuração,
 * um Access Point é criado e este servidor permite
 * configurar parâmetros como WiFi e MQTT.
 */
ESP8266WebServer server(80);

// -----------------------------------------------------------------------------
// Funções de navegação entre chamadas
// -----------------------------------------------------------------------------

/**
 * @brief Manipula o botão "Next" (próxima chamada).
 *
 * @details
 * Caso o sistema esteja em modo de confirmação de exclusão,
 * o clique cancela a operação.
 *
 * Caso contrário, avança para o próximo item da lista
 * de chamadas e atualiza o display.
 */
void handleNext() {  // ===== Botão Next (>)
  // Se estava em modo confirmação de deleção, atualiza o display, mostrando que a ação foi cancelada
  if (deletionConfirmation) {
    deletionConfirmation = false;  // reset caso usuário navegue (cancela a exclusão)

    // Quando clica 'next' e esta no esta no modo confirmação, garante que NÃO está bloqueado remover current (caso tenha atigido o limite de inserção na lista)
    listCalls.setDoNotRemoveCurrent(false);  // vira false -> pode remover current
  } else {
    if (listCalls.hasNursingCall()) {
      // Avança para o próximo item da lista
      listCalls.next();
      log(LOG_DEBUG, listCalls.getIdCurrent());
    }
  }
  // Mostra o item atual
  showInfirmaryNumber(
    listCalls.getInfirmaryCurrent(),
    listCalls.hasNursingCall(),
    listCalls.getTotal());
}

/**
 * @brief Manipula o botão "Previous" (chamada anterior).
 *
 * @details
 * Similar ao botão Next, mas navega para o item anterior
 * da lista de chamadas.
 */
void handlePrev() {  // ===== Botão Prev (<)
  // Se estava em modo confirmação de deleção, atualiza o display, mostrando que a ação foi cancelada
  if (deletionConfirmation) {
    deletionConfirmation = false;  // reset caso usuário navegue (cancela a exclusão)

    // Quando clica 'prev' e esta no esta no modo confirmação, garante que NÃO está bloqueado remover current (caso tenha atigido o limite de inserção na lista)
    listCalls.setDoNotRemoveCurrent(false);  // vira false - pode remover current
  } else {
    if (listCalls.hasNursingCall()) {
      // Avança para o item anteriot da lista
      listCalls.prev();
      log(LOG_DEBUG, listCalls.getIdCurrent());
    }
  }
  // Mostra o item atual
  showInfirmaryNumber(
    listCalls.getInfirmaryCurrent(),
    listCalls.hasNursingCall(),
    listCalls.getTotal());
}

/**
 * @brief Manipula o botão de exclusão de chamada.
 *
 * @details
 * O processo ocorre em duas etapas:
 *
 * 1º clique:
 * - Mostra tela de confirmação de exclusão.
 *
 * 2º clique:
 * - Publica no MQTT a confirmação de resolução
 *   do chamado.
 */
void handleDelete() {  // ===== Botão Delete
  // Primeiro clique: apenas exibe a mensagem de confirmação
  if (!deletionConfirmation) {

    // Se a tela anterior não for a MAIN, não pode mostrar a tela de confirmação de exclusão
    if (currentScreen != SCREEN_MAIN) {
      return;
    }

    showExclusionConfirm(listCalls.getInfirmaryCurrent());
    deletionConfirmation = true;

    // Quando clica 1 vez no 'del' não pode remover current, caso tenha atigido o limite de inserção na lista
    listCalls.setDoNotRemoveCurrent(true);  // Aqui diz é true, não pode remover current
  } else {                                  // Segundo clique: executa deleção

    // Se a tela anterior não for a de confirmação de exclusão, não pode continuar
    if (currentScreen != SCREEN_EXCLUSION_CONFIRM) {
      return;
    }

    /* ___Pública (marcar com concluído o chamado) via MQTT*/
    const char *infirmary = listCalls.getInfirmaryCurrent();
    const char *idDevice = listCalls.getIdCurrent();
    // Chama a função de públicar o ID do dispositivo e o número da enfermaria (tranforma em float o infirmary)

    char buffer[256];

    // flag qu indica se deu certo publicar ou não
    bool wasPublished = publicReponseDivice(idDevice, MQTT_PUBLICATION_TOPIC, createJsonPayload(buffer, sizeof(buffer), "atendido"));

    // Caso tenha falhado publicar o chamado, mostra a tela indicando a falha
    if (!wasPublished) {
      showFailureMessage(MESSAGE_MQTT);
    }

    log(LOG_INFO, "enfermaria a ser apagada foi enviada");
  }
}

// -----------------------------------------------------------------------------
// Inicialização do sistema
// -----------------------------------------------------------------------------

/**
 * @brief Inicializa todos os módulos do sistema.
 *
 * Executado apenas uma vez após o boot do microcontrolador.
 *
 * Inicializa:
 * - sistema de logs
 * - display LCD
 * - botões
 * - LED e buzzer
 * - armazenamento de configuração
 * - conexão WiFi
 * - comunicação MQTT
 */
void setup() {
  // Serial.begin(115200);
  logInit(LOG_MODE);
  // inicializa o display
  lcd2004_init();

  // Inicializa botões
  initButtons();

  enableButtonInterruptChange();

  ledInit();

  // inicializa buzzer
  buzzerInit();

  initConfigStorage();
  cfg = loadConfig();

  if (!connectToWiFi()) {
    // Serial.println("WiFi não conectado.");
    log(LOG_WARN, "Falha ao conectar com WiFI.");
  }

  setupMQTT();

  showInfirmaryNumber(
    listCalls.getInfirmaryCurrent(),
    listCalls.hasNursingCall(),
    listCalls.getTotal());  // Mostra os dados no display
}

// -----------------------------------------------------------------------------
// Loop principal do firmware
// -----------------------------------------------------------------------------

/**
 * @brief Loop principal do firmware.
 *
 * @details
 * Executado continuamente enquanto o dispositivo estiver ligado.
 *
 * Responsável por:
 *
 * - Gerenciar modo de configuração (Access Point + servidor web)
 * - Manter conexão WiFi e MQTT
 * - Processar mensagens de confirmação
 * - Atualizar display quando necessário
 * - Tratar botões de navegação
 * - Controlar LED e buzzer
 */
void loop() {

  // -------------------------------------------------------------------------
  // Modo de configuração
  // -------------------------------------------------------------------------

  if (isConfigurationMode() || !cfg.valid) {
    log(LOG_DEBUG, "isconfigurationMode %d",isConfigurationMode());
    log(LOG_DEBUG, "cfgValid %d",!cfg.valid);

    turnOnLed();
    createAccessPoint();
    startServer(&server);
    server_handle_loop(&server);

    // log(LOG_INFO, "modo configuraçao");
  } else {

    // ---------------------------------------------------------------------
    // Conexões de rede
    // ---------------------------------------------------------------------

    if (checkAndReconnectWifi()) {

      if (!checkMQTTConnected()) {
        listCalls.clear();
      }
    } else {                             // não há conexão
      showFailureMessage(MESSAGE_WIFI);  // Mensagem que indica que não há conexão Wi-Fi**
    }

    stopServer(&server);

    // ---------------------------------------------------------------------
    // Processamento de mensagens de confirmação MQTT
    // ---------------------------------------------------------------------


    if (newMessage) {
      newMessage = false;

      if (strncmp(receivedTopic, MQTT_SUBSCRIPTION_TOPIC, strlen(MQTT_SUBSCRIPTION_TOPIC)) == 0) {
        PatientStatus patientStatus = {};

        if (processing_json_MQTT((byte *)receivedPayload, receivedLength, &patientStatus)) {

          if (strcmp(patientStatus.estado, "emergencia") == 0) {

            // Adiciona na lista com verificação de sucesso
            if (listCalls.add(patientStatus.room_number, patientStatus.id)) {
              listUpdated = true;  // sinaliza que display precisa atualizar
            } else {
              log(LOG_ERROR, "Erro ao adicionar chamada na lista!");
              return;
            }

            char buffer[90];
            publicReponseDivice(
              patientStatus.id,
              MQTT_PUB_CONFIRMATION_TOPIC,
              creteJsonPayloadConfirmationMessage(buffer, sizeof(buffer)),
              true);

            enableSoundAlert();
          } else if (strcmp(patientStatus.estado, "ocioso") == 0) {

            if (listCalls.removalById(patientStatus.id)) {
              publicReponseDivice(patientStatus.id, MQTT_PUB_CONFIRMATION_TOPIC, "", true);
              char buffer[150];
              publicReponseDivice(patientStatus.id, MQTT_PUBLICATION_TOPIC, createJsonPayload(buffer, sizeof(buffer), "finalizar"));
              log(LOG_INFO, "Chamada removida com sucesso!");

              showInfirmaryNumber(
                listCalls.getInfirmaryCurrent(),
                listCalls.hasNursingCall(),
                listCalls.getTotal());  // Mostra os dados no display

              // Ao marcar o chamado como resolvido, reseta a flag, indicando se atingir o limite pode remover o current
              deletionConfirmation = false;
              listCalls.setDoNotRemoveCurrent(false);  // vira false - pode remover current

            } else {
              log(LOG_ERROR, "Erro ao remover a chamada na lista!");
            }

          } else {
            log(LOG_INFO, "json com campo estado fora do comum");
          }
        } else {
          log(LOG_INFO, "json não processado");
        }
      }
    }
  }

  // ---------------------------------------------------------------------
  // Atualização do display
  // ---------------------------------------------------------------------

  if (listUpdated
      && button_next.state == HIGH
      && button_prev.state == HIGH
      && button_delete.state == HIGH
      && currentScreen == SCREEN_MAIN
      && !deletionConfirmation) {


    showInfirmaryNumber(
      listCalls.getInfirmaryCurrent(),
      listCalls.hasNursingCall(),
      listCalls.getTotal());

    listUpdated = false;  // reseta a flag
  }

  // ---------------------------------------------------------------------
  // Leitura dos botões
  // ---------------------------------------------------------------------

  if (checkButton(button_next))
    handleNext();
  if (checkButton(button_prev))
    handlePrev();

  // ---------------------------------------------------------------------
  // Controle de LED e botão de exclusão
  // ---------------------------------------------------------------------

  if (listCalls.hasNursingCall()) {
    // if (checkButton(button_next)) handleNext();
    // if (checkButton(button_prev)) handlePrev();
    if (checkButton(button_delete))
      handleDelete();

    toggleLed();
  } else {
    turnOffLed();
  }

  // ---------------------------------------------------------------------
  // Controle do buzzer
  // ---------------------------------------------------------------------

  if (doesHaveNotificationBuzzer()) {
    toggleBuzzer();
  }
}
