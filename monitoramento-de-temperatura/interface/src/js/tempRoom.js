
const params = new URLSearchParams(window.location.search);
const salaAtual = params.get("sala");

import { buscarTemperaturaSala } from './api.js';
import { createGauge } from './gauge.js';


//carrega e atualiza a temperatura da sala a partir do id
async function roomUpdate() {
  try {
    const dados = await buscarTemperaturaSala(salaAtual);

    if (!dados.length) {
      console.error(`Nenhum dado encontrado para ${salaAtual}`);
      return;
    }

    const ultimaLeitura = dados[dados.length - 1];

    const grid = document.getElementById("salaGrid");
    grid.innerHTML = "";

    const tempGaugeId = `gauge-temp-${salaAtual}`;
    const humGaugeId = `gauge-hum-${salaAtual}`;

    const card = document.createElement("div");
    card.className = "monitor"; // container principal

    card.innerHTML = `
    

      <div class="temp">
        <div id="${tempGaugeId}" style="width:320px; height:240px;"></div>
        <h4>Temperatura</h4>
      </div>
      <div class="umid">
        <div id="${humGaugeId}" style="width:320px; height:240px;"></div>
        <h4>Umidade</h4>
      </div>
    </div>  
    
`;


    grid.appendChild(card);

    createGauge(tempGaugeId, humGaugeId, ultimaLeitura);

  } catch (err) {
    console.error("Erro ao carregar sala:", err);
  }
}

roomUpdate();
setInterval(roomUpdate, 60000); // atualiza de 1 em 1 minuto