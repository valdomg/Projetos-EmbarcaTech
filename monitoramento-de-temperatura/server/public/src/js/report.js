
import { roomTempPDF, roomsSearch } from "./api.js";
import { alertMsg } from "./main.js";

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
  console.log(start);
  console.log(end);

  if (!idRoom){
    const msgAlert = "É necessário informar o local.";
    alertMsg('alertRelatorio', msgAlert,'erro');

    return;
  }

  try {
    const msgAlert1 = "Gerando relatório, por favor aguarde!";
    alertMsg('alertRelatorio', msgAlert1,'sucesso');
    const dados = await roomTempPDF(idRoom, start, end);
    console.log(dados);

    if (dados.erro) {
      const msgAlert = dados.erro;
      alertMsg('alertRelatorio', msgAlert,'erro');

      return;
    }

    const linkPDF = dados.link;
    
    const msgAlert = (`${dados.message}. <a href="${linkPDF}" target="_blank">Para visualizar clique aqui!</a>`);

    alertMsg('alertRelatorio', msgAlert,'sucesso', null);
    


  } catch (erro){
    const msgAlert = erro.message;
    alertMsg('alertRelatorio', msgAlert,'erro');
  }
};


