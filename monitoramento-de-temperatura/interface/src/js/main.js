import { checkAcess } from './auth.js';
import { buscarTemperaturas } from './api.js';
import { createGauge } from './components/gauge.js';


document.addEventListener("DOMContentLoaded", () => {
  checkAcess();
});

//carrega dados de temperatura de todas as salas cadastradas no sistema.
export async function carregarTemperaturas() {

  try {
    const dados = await buscarTemperaturas();

    if (!dados.length) {
      console.error("Nenhum dado encontrado.");
      return;
    }

    // Agrupa por sala e pega a última leitura
    const ultimasPorSala = {};
    dados.forEach(dado => {
      const sala = dado.room; 
      if (!sala){
        return;
      }
      if (!ultimasPorSala[sala._id] || new Date(dado.timestamp) > new Date(ultimasPorSala[sala._id].timestamp)) {
        ultimasPorSala[sala._id] = dado;
      }
    });

    // Transforma em array
    const salas = Object.values(ultimasPorSala);

    sortRoom(salas);



    // Renderiza grid
    const grid = document.getElementById("roomsGrid");
    grid.innerHTML = "";

    salas.forEach(sala => {
      const card = document.createElement("div");
      card.className = "room-card";
      card.onclick = () => abrirSala(sala.room._id);

      // ids únicos para cada gauge
      const tempGaugeId = `gauge-temp-${sala.room._id}`;
      const humGaugeId = `gauge-hum-${sala.room._id}`;

      card.innerHTML = `
    <h3>${sala.room.name.toUpperCase()}</h3>
    <div id="${tempGaugeId}" style="width:200px; height:200px;"></div>
    
    <p><em>Última atualização: ${new Date(sala.timestamp).toLocaleString("pt-BR")}</em></p>
  `;

      grid.appendChild(card);

      const body = document.body;
      body.classList.remove('hide');
      body.classList.add('show');

      // cria o gauge de temperatura
      createGauge(tempGaugeId, humGaugeId, sala);
    });

  } catch (error) {
    console.error("Erro ao carregar salas:", error);

    if (error.message.includes("401") || error.message.includes("403")) {
      alert("Acesso não autorizado. Faça login novamente.");
      localStorage.removeItem("token");
      window.location.href = "login.html";
    }
  }
}


carregarTemperaturas();
setInterval(carregarTemperaturas, 60000); // atualiza a cada 1min


//redirecionamento para pagina de uma sala apartir do id
function abrirSala(salaId) {
  window.location.href = `room.html?sala=${salaId}`;
}

// ordenar em ordem crescente pelo nome da sala 
function sortRoom(salas) {
  salas.sort((a, b) => {
    // extrai apenas os números do nome (ex: sala-01 -> 1)
    const numA = parseInt(a.room.name.replace(/\D/g, ''), 10);
    const numB = parseInt(b.room.name.replace(/\D/g, ''), 10);

    return numA - numB;
  });
}


//eventListener de logout
document.getElementById("logoutBtn").addEventListener("click", async function () {
  localStorage.removeItem("token");
  window.location.href = "login.html";
});
