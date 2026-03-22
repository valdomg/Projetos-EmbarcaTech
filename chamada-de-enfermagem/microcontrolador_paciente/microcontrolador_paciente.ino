/**
 * @file microcontrolador_paciente.ino
 * @brief Firmware principal do módulo paciente do sistema de chamada de enfermagem.
 *
 * @details
 * Este firmware é responsável por:
 * - Inicializar periféricos (botão e LED);
 * - Estabelecer conexão Wi-Fi;
 * - Conectar ao broker MQTT;
 * - Monitorar o botão do paciente;
 * - Publicar uma solicitação MQTT quando o botão for pressionado.
 *
 * Arquitetura modular:
 * - wifi.h   → Gerenciamento de conexão Wi-Fi
 * - mqtt.h   → Comunicação com broker MQTT
 * - button.h → Leitura do botão físico
 * - led.h    → Controle do LED indicador
 */

#include "wifi.h"
#include "mqtt.h"
#include "button.h"
#include "led.h"

/**
 * @brief Indica se o botão está habilitado para enviar uma nova solicitação.
 *
 * @details
 * - false → botão liberado para envio
 * - true  → botão bloqueado após envio bem-sucedido
 *
 * Essa variável evita múltiplos envios consecutivos enquanto
 * o botão permanece pressionado.
 */
bool buttonEnable = false;


/**
 * @brief Função de inicialização do sistema.
 *
 * @details
 * Executada uma única vez ao iniciar o microcontrolador.
 * Responsável por:
 * - Inicializar comunicação serial;
 * - Configurar periféricos;
 * - Inicializar cliente MQTT;
 * - Conectar à rede Wi-Fi.
 */
void setup() {

  /// Inicializa comunicação serial para debug
  Serial.begin(115200);

  /// Inicializa periféricos de entrada e saída
  setupButton();
  setupLed();

  /// Inicializa cliente MQTT e conecta ao Wi-Fi
  setupMQTT();
  connectWiFi();
}


/**
 * @brief Loop principal do firmware.
 *
 * @details
 * Executado continuamente após o setup().
 * 
 * Fluxo principal:
 * 1. Garante conexão MQTT;
 * 2. Processa loop interno do cliente MQTT;
 * 3. Verifica conexão Wi-Fi;
 * 4. Monitora botão e publica mensagem quando necessário.
 */
void loop() {

  /// Garante que o cliente MQTT esteja conectado
  if (!client.connected()) {
    connectMQTT();
  }

  /// Processa o loop interno do cliente MQTT
  client.loop();

  /// Verifica estado da conexão Wi-Fi
  checkConnection();

  /// Executa lógica principal apenas quando conectado ao broker
  if (client.connected()) {

    /**
     * Envia solicitação apenas se:
     * - O botão estiver liberado
     * - O botão estiver pressionado
     *
     * Observação:
     * readButton() retorna LOW quando pressionado
     * (configuração INPUT_PULLUP)
     */
    if (!buttonEnable && !readButton()) {

      /// Publica mensagem no tópico MQTT configurado
      if (client.publish(TOPIC_PUBLISH, createJsonPayload("emergencia", "atender", "ligar"), true)) {

        Serial.println("Solicitação enviada");

        /// Bloqueia novo envio até liberação
        buttonEnable = true;

      } else {

        Serial.println("Falha ao enviar solicitação");

        /// Mantém botão liberado em caso de falha
        buttonEnable = false;
      }
    }
  }
}