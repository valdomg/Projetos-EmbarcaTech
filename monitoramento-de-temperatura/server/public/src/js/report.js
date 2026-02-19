
import { roomTempInterval, roomsSearch } from "./api.js";

//gerar opções do menu relario dinamicamente
async function gerarOptions() {

  try {
      const dados = await roomsSearch();
      
      const select = document.getElementById('room-select');
      dados.forEach(sala => {
      select.innerHTML += `<option value="${sala._id}">${sala.name}</option>`;
    });
  }catch(erro){
    console.error("Erro ao carregar opções do menu:", erro);
  }
}

//so gera as opções do menu se houver o id no html
if (document.getElementById('room-select')) {
  gerarOptions();
}



const emitir = document.getElementById('emitirRelatorio');
// evento clique chamando a funcao gerar relatorio
if (emitir){
  emitir.addEventListener('click', async ()=>{

    gerarRelatorio();
  })
}


export async function gerarRelatorio() {

    // Converte os valores dos inputs em ISO
  const idRoom = document.querySelector('select').value;
  const start = document.getElementById('start').value; // YYYY-MM-DD
  const end = document.getElementById('end').value;     // YYYY-MM-DD

  try {
    const dados = await roomTempInterval(idRoom, start, end);
    console.log(dados);

    if (!dados.length) {
      window.alert("Sem dados para o período");
      return;
    }

    // Monta CSV
    const csvHeader = 'Data;Temperatura;Umidade\n';
    const csvRows = dados.map(salas => {
      const data = new Date(salas.timestamp).toLocaleString("pt-BR");
      const temp = salas.temperature || '';
      const umid = salas.humidity || '';
      return `${data};${temp};${umid}`;
    });

    const csvContent = csvHeader + csvRows.join('\n');
    downloadCSV(csvContent, `relatorio_${start}_${Date.now()}.csv`);

  } catch (erro){
    window.alert(` ${erro.message}`);
  }
};


//funcao para baixar arquivo csv
function downloadCSV(content, filename) {
  const blob = new Blob([content], { type: 'text/csv;charset=utf-8;' });
  const link = document.createElement("a");
  if (link.download !== undefined) {
    const url = URL.createObjectURL(blob);
    link.setAttribute("href", url);
    link.setAttribute("download", filename);
    document.body.appendChild(link);
    link.click();
    document.body.removeChild(link);
  }
}

//eventListener de logout
document.getElementById("logoutBtn").addEventListener("click", async function () {
  localStorage.removeItem("token");
  window.location.href = "login.html";
});