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
      const grid = document.getElementById("dashboard");

      if (grid) {
        grid.innerHTML = "<p>Nenhuma sala com dados disponíveis</p>";
      }

      return;
    }

    // Agrupa por sala e pega a última leitura
    const ultimasPorSala = {};
    dados.forEach(dado => {
      const sala = dado.room;
      if (!sala) {

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

    // condição caso o grid tenha sido removido do html
    if (!grid) {
      console.log('Não encontrou o elemento RoomGrid');
      return;
    }
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


//função abrir e fechar modal
function abrirModal(modalId, closeBtnSelector) {
  // Mostrar o modal
  const modal = document.getElementById(modalId);
  if (!modal) {
    console.log(`Modal com ID "${modalId}" não encontrado.`);
    return;
  }
  modal.showModal();

  //Fechar modal
  const btnClose = modal.querySelector(closeBtnSelector);
  if (!btnClose) {
    console.log(`Modal com ID "${closeBtnSelector}" nao encontrado`)
    return;
  }
  btnClose.onclick = () => modal.close();
}

//Função logout usuário
function logoutUser() {
  // Confirmar exclusão
  // Mostrar o modal
  const modal = document.getElementById('modalLogout');
  abrirModal('modalLogout', '#logoutNo');

  // Mensagem final
  const mensagem = "Tem certeza que deseja sair?";
  document.getElementById('logoutMessage').innerText = mensagem;

  const logout = document.getElementById("logoutYes");
  logout.onclick = async (e) => {
    e.preventDefault();

    modal.close();
    localStorage.removeItem("token");
    window.location.href = "login.html";
  }
}


//eventListener de logout
document.getElementById("logoutBtn").addEventListener("click", async function () {

  logoutUser();
});


