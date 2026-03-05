const vermelho = "#c91f1079"
const verde = "#87c454"
const azul = "#4384c4"

let dados_mapa;

function renderMapa(containerId) {
    const container = document.getElementById(containerId);
    container.innerHTML = '';

    let posicaoMapa = 0
    
    dados_mapa.forEach(dado => {
      div = document.createElement("div")
      div.className = 'celula-enfermaria'
      div.id = dado._id;

      

      div.appendChild(Object.assign(document.createElement("p"), {
        textContent: `Dispositivo: ${dado.device}`
      }));
  
      div.appendChild(Object.assign(document.createElement("p"), {
        textContent: `Sala: ${dado.room_number}`
      }));

      div.appendChild(Object.assign(document.createElement("p"), {
        textContent: `Estado: ${dado.status}`
      }));

      div.appendChild(Object.assign(document.createElement("p"), {
        textContent: `Local: ${dado.local}`
      }));

      dataRAW =  dado.updateAt.replace("ISODate('", "").replace("')", "");
      data = new Date(dataRAW);

      div.appendChild(Object.assign(document.createElement("p"), {
        textContent: `Última atualização: ${data.toLocaleString("pt-BR", { timeZone: "UTC" })}`
      }));

      if (dado.status == "emergencia") {
        div.style.backgroundColor = vermelho;

        const botao = document.createElement("button");

        botao.textContent = "Encerrar Chamada";
        botao.dataset.id = posicaoMapa

        botao.addEventListener("click", (event) => {
          resolverEmergencia(event.currentTarget.dataset.id);
        })

        div.appendChild(botao);
      }

      container.appendChild(div);
      posicaoMapa++;

    });
}

async function fetchMapa(containerId = "mapa") {
    try {
        const response = await fetch("/api/mapa/status"); 
        if (!response.ok) {
            throw new Error("Erro ao buscar mapa");
        }

        dados_mapa = await response.json(); 
        renderMapa(containerId);

    }
     catch (error) {
        console.error(error);
    }   
}

async function resolverEmergencia(id) {
  try {
    dadosEmergenciaRAW = dados_mapa[id];
  
    const { device, room_number, local } = dadosEmergenciaRAW;
  
    const dadosEmergencia = { device, room_number, local };
  
    // console.log(dadosEmergencia)
    const resposta = await fetch('/api/mapa/mensagem/ocioso', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(dadosEmergencia)
    });
  
    if (!resposta.ok) {
      const body = await resposta.json();
      alert(`Erro ao atualizar mapa. ${body.Error}`);
      return;
    }
    fetchMapa("mapa");
  
  } catch (error) {
    alert(`Erro ao atualizar mapa`);
  }

}

fetchMapa("mapa");

setInterval(fetchMapa, 10000)
