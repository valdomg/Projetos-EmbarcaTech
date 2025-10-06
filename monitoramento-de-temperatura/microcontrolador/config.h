#ifndef CONFIG_H
#define CONFIG_H

// -----------------------------------------------------------------------------
// Constantes de configuração do MQTT
// -----------------------------------------------------------------------------

/// Endereço do broker MQTT.
constexpr char* MQTT_SERVER = "xxxxxxxxxxx";

/// Usuário para autenticação no broker MQTT.
constexpr char* MQTT_USER = "xxxxxxxxxxxx";

/// Senha para autenticação no broker MQTT.
constexpr char* MQTT_PASS = "xxxxxxxxxxxx";

/// Tópico para publicação de dados de sensores (temperatura/umidade).
constexpr char* MQTT_TOPIC_PUBLICATION_DATA = "xxxxxxxxxxxx/xxxxxxxxxxxx";

/// Tópico para publicação de alertas.
constexpr char* MQTT_TOPIC_PUBLICATION_ALERT = "xxxxxxxxxxxx/xxxxxxxxxxxx";

/// Identificação única do dispositivo no broker MQTT.
constexpr char* MQTT_DEVICE_ID = "xxxxxxxxxxxx";


#endif