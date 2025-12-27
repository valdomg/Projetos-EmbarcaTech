const contagemChamada = document.getElementById("contagemChamada")
const contagemDispositivos = document.getElementById("contagemDispositivos")
const ChamadasTodas = document.querySelector("#allChamadas")
const chamdasDia = document.getElementById("chamadas-lista");

function renderChamadas(dados) {

  chamdasDia.innerHTML = ''

  dados.forEach(item => {
    const div = document.createElement("div");
    div.className = "chamada-exibir";

    div.appendChild(Object.assign(document.createElement("p"), {
      textContent: `Dispositivo: ${item.dispositivo_id}`
    }));

    div.appendChild(Object.assign(document.createElement("p"), {
      textContent: `Local: ${item.local}`
    }));

    // data = new Date(item.data)
    dataRAW =  item.data.replace("ISODate('", "").replace("')", "");
    data = new Date(dataRAW);
    div.appendChild(Object.assign(document.createElement("p"), {
      textContent: `Hora: ${data.toLocaleTimeString("pt-BR", { timeZone: "UTC" })}`
    }));

    chamdasDia.appendChild(div);
  });
}

function renderChamadasTodas(dados) {
  let indice = 1

  ChamadasTodas.innerHTML = ''
  dados.forEach(chamada =>
  {
    const tr = document.createElement("tr")

    tr.appendChild(Object.assign(document.createElement("th"), {
        scope: "row",
        innerHTML: indice
    }))
    indice++;

    tr.appendChild(Object.assign(document.createElement("td"), {
        innerHTML: `${chamada.dispositivo_id}`
    }))

    tr.appendChild(Object.assign(document.createElement("td"), {
        innerHTML: `${chamada.local}`
    }))

    // tr.appendChild(Object.assign(document.createElement("th"), {
    //     innerHTML: `${chamada.sala}`
    // }))

    dataRAW =  chamada.data.replace("ISODate('", "").replace("')", "");
    data = new Date(dataRAW);
    tr.appendChild(Object.assign(document.createElement("td"), {
        innerHTML: `${data.toLocaleString("pt-BR", { timeZone: "UTC" })}`
    }))
    
    ChamadasTodas.appendChild(tr);
  })}

async function fetchChamadas() {
    try {
        const response = await fetch("/api/chamadas/dia"); 
        if (!response.ok) {
            throw new Error("Erro ao buscar chamadas");
        }

        const dados = await response.json(); 
        renderChamadas(dados);

    }
     catch (error) {
        console.error(error);
    }   

    try {
      const response = await fetch("/api/chamadas"); 
        if (!response.ok) {
            throw new Error("Erro ao buscar chamadas");
        }

        const dados = await response.json(); 
        renderChamadasTodas(dados);

    } catch (error) {
        console.error(error);
    } 

    try {
        const response = await fetch("/api/chamadas/dia/contagem"); 
        const dados = await response.json()

        contagemChamada.innerHTML = dados.Quantidade;
    } catch (error) {
      console.error(error);
    }

    try {
        const response = await fetch("/api/devices/quantidade"); 
        const dados = await response.json()

        contagemDispositivos.innerHTML = dados.Quantidade;
    } catch (error) {
      console.error(error);
    }
}

fetchChamadas("chamadas-lista");

setInterval(fetchChamadas, 30000)