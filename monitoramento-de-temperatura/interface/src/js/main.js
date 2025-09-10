
import { buscarTemperaturas } from './api.js';
import { createGauge } from './gauge.js';


//carrega dados de temperatura de todas as salas cadastradas no sistema.
async function carregarTemperaturas() {
  try {
    const dados = await buscarTemperaturas();

    if (!dados.length) {
      console.error("Nenhum dado encontrado.");
      return;
    }

    // Agrupa por sala e pega a última leitura
    const ultimasPorSala = {};
    dados.forEach(dado => {
      const sala = dado.room; // certifique-se que o campo no JSON se chama "room"
      if (!ultimasPorSala[sala] || new Date(dado.timestamp) > new Date(ultimasPorSala[sala].timestamp)) {
        ultimasPorSala[sala] = dado;
      }
    });

    // Transforma em array
    const salas = Object.values(ultimasPorSala);


    // Renderiza grid
    const grid = document.getElementById("salasGrid");
    grid.innerHTML = "";

    salas.forEach(sala => {
      const card = document.createElement("div");
      card.className = "temp-sala";
      card.onclick = () => abrirSala(sala.room);

      // ids únicos para cada gauge
      const tempGaugeId = `gauge-temp-${sala.room}`;
      const humGaugeId = `gauge-hum-${sala.room}`;

      card.innerHTML = `
    <h3>${sala.room.toUpperCase()}</h3>
    <div id="${tempGaugeId}" style="width:200px; height:200px;"></div>
    
    <p><em>Última atualização: ${new Date(sala.timestamp).toLocaleString("pt-BR")}</em></p>
  `;

      grid.appendChild(card);

      // cria o gauge de temperatura
      createGauge(tempGaugeId, humGaugeId, sala);
    });

  } catch (error) {
    console.error("Erro ao carregar salas:", error);
  }
}


carregarTemperaturas();
setInterval(carregarTemperaturas, 60000); // atualiza a cada 10s