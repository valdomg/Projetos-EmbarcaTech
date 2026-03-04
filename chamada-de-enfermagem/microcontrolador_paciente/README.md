README# ⚡ Microcontrolador Paciente – Sistema de Chamada de Enfermagem

## 📝 Descrição
Este projeto implementa o firmware para o módulo paciente do sistema de chamada de enfermagem, utilizando **ESP8266**, **MQTT** e **LED/botão** como interface com o paciente.  
O sistema permite que o paciente solicite atendimento, enviando uma mensagem via MQTT para o **posto de enfermagem**, com confirmação visual através do LED.

## 🎯 Objetivo
- ✅ Prover um dispositivo seguro e confiável para chamadas de emergência  
- 🔄 Implementar comunicação bidirecional com o posto de enfermagem via MQTT  
- 🛠 Facilitar manutenção e expansão através de código modular

## 🧩 Componentes Utilizados
- 💻 ESP8266 (NodeMCU ou equivalente)  
- 💡 LED indicador  
- 🔘 Botão de chamada  

## 📂 Estrutura do Código
O firmware está organizado em módulos:

- **wifi** → Gerenciamento da conexão Wi-Fi (`wifi.cpp`, `wifi.h`)  
- **mqtt** → Comunicação com o broker MQTT (`mqtt.cpp`, `mqtt.h`)  
- **button** → Leitura do botão do paciente (`button.cpp`, `button.h`)  
- **led** → Controle do LED indicador (`led.cpp`, `led.h`)  
- **microcontrolador_paciente.ino** → Arquivo principal integrando todos os módulos  

## ⚙️ Funcionamento
O fluxo do sistema de chamada é o seguinte:

1. **🟢 Solicitação de atendimento:**  
   - O paciente pressiona o botão no microcontrolador  
   - O dispositivo envia uma mensagem JSON via MQTT para o **microcontrolador central**  
   - O botão é temporariamente bloqueado para evitar múltiplos envios

2. **📩 Confirmação de recebimento:**  
   - Quando o sistema do posto de enfermagem recebe a solicitação, envia uma mensagem `"ok"` de volta  
   - Ao receber essa confirmação, o **LED do paciente acende**, indicando que o chamado foi registrado com sucesso

3. **🔴 Finalização do atendimento:**  
   - A enfermeira finaliza o chamado enviando uma mensagem `"desligar"`  
   - O LED é desligado e o botão liberado para novas solicitações  
   - O microcontrolador do paciente envia `"finalizado"` para o central, para remover o chamado da fila

> Esse fluxo garante comunicação bidirecional confiável, status visual imediato e controle seguro do botão.

## 🛠 Protótipo e Conexões
Abaixo está o diagrama do protótipo mostrando as conexões físicas dos componentes (gerado no Fritzing):

![Protótipo Fritzing](./hardware/prototipo.png)