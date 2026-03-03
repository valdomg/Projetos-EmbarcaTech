
const params = new URLSearchParams(window.location.search);
const salaAtual = params.get("sala");

import { checkAcess, logoutUser } from './auth.js';
import { buscarTemperaturaSala, roomTempInterval } from './api.js';
import { createGauge } from './components/gauge.js';
import { tempChart, umidChart } from './components/charts.js';


document.addEventListener("DOMContentLoaded", () => {
  checkAcess();
});

//carrega e atualiza a temperatura da sala a partir do id
async function roomUpdate() {

  try {
    const token = localStorage.getItem("token");

    const decode = jwt_decode(token);
    const dados = await buscarTemperaturaSala(salaAtual);

          const grid = document.getElementById("roomGrid");

    if (!dados.length) {
      console.error(`Nenhum dado encontrado para ${salaAtual}`);
      return;
    }



    // condição caso o grid tenha sido removido do html
    if (!grid) {
      console.log('Não encontrou o elemento RoomGrid');
      return;
    }



    if (decode.role == "admin") {
      const body = document.body;
      body.classList.remove('hide');
      body.classList.add('show');
      body.classList.add('default');
      const navbar = document.getElementById('navbar');
      const link = document.getElementById('link');
      link.href = 'admin.html';
      navbar.classList.add('navbar');

      const ultimaLeitura = dados[dados.length - 1];

      const grid = document.getElementById("roomGrid");
      grid.innerHTML = "";

      const tempGaugeId = `gauge-temp-${salaAtual}`;
      const humGaugeId = `gauge-hum-${salaAtual}`;

      grid.innerHTML = `
              <div class="temp">
                <div id="${tempGaugeId}" style="width:320px; height:240px;"></div>
                <h4 class="dark">Temperatura</h4>
              </div>
              <div class="umid">
                <div id="${humGaugeId}" style="width:320px; height:240px;"></div>
                <h4 class="dark">Umidade</h4>
              </div>
          </div>    
        `;
      createGauge(tempGaugeId, humGaugeId, ultimaLeitura);

    } else {
      const body = document.body;
      body.classList.remove('hide');
      body.classList.add('show');

      const ultimaLeitura = dados[dados.length - 1];

      const grid = document.getElementById("roomGrid");
      const link = document.getElementById('link');
      link.href = 'index.html';
      grid.innerHTML = "";

      const tempGaugeId = `gauge-temp-${salaAtual}`;
      const humGaugeId = `gauge-hum-${salaAtual}`;

      grid.innerHTML = `
              <div class="temp">
                <div id="${tempGaugeId}" style="width:320px; height:240px;"></div>
                <h4 class="dark">Temperatura</h4>
              </div>
              <div class="umid">
                <div id="${humGaugeId}" style="width:320px; height:240px;"></div>
                <h4 class="dark">Umidade</h4>
              </div>
          </div>    
        `;
      createGauge(tempGaugeId, humGaugeId, ultimaLeitura);

    }


  } catch (error) {
    console.error("Erro ao carregar sala:", error);
  }
}

roomUpdate();
setInterval(roomUpdate, 60000); // atualiza de 1 em 1 minuto



//Atualiza dados do grafico
async function atualizarGraficos() {
  try {
    const { start, end } = calcularIntervalo();
    const dados = await roomTempInterval(salaAtual, start, end);

    const grafico = document.querySelector(".chart");

    if (!Array.isArray(dados) || !dados || dados.length === 0) {
      console.log("Nenhum dado retornado para o gráfico.");
      grafico.innerHTML = '<h4 class="dark">Sem dados nas últimas 24 horas!</h4>'
      return;
    }

    // Ordenar por timestamp
    dados.sort((a, b) => new Date(a.timestamp) - new Date(b.timestamp));

    console.log(dados);

    const labels = [];
    const temperaturas = [];
    const umidades = [];

    dados.forEach(item => {
      const date = new Date(item.timestamp);
      const hora = date.getHours().toString().padStart(2, "0");
      const minutos = date.getMinutes().toString().padStart(2, "0");
      const horaFormatada = `${hora}:${minutos}h`;

      labels.push(horaFormatada);
      temperaturas.push(item.temperature);
      umidades.push(item.humidity);
});

    // Atualiza gráfico de temperatura
    tempChart.data.labels = labels;
    tempChart.data.datasets[0].data = temperaturas;
    tempChart.update();

    // Atualiza gráfico de umidade
    umidChart.data.labels = labels;
    umidChart.data.datasets[0].data = umidades;
    umidChart.update();

  } catch (err) {
    console.error("Erro ao atualizar gráficos:", err);
  }
}

function calcularIntervalo() {
  const agora = new Date();

  const start = new Date(agora);
  start.setDate(start.getDate() - 1);

  const startDate = start.toISOString().split("T")[0];
  const endDate = agora.toISOString().split("T")[0];

  return {
    start: startDate, // YYYY-MM-DD
    end: endDate      // YYYY-MM-DD
  };
}


// Inicializa e atualiza a cada minuto
atualizarGraficos();
setInterval(atualizarGraficos, 60000);


//eventListener de logout
document.getElementById("logoutBtn").addEventListener("click", async function () {

  logoutUser();
});
