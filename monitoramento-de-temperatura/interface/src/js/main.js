
import { buscarTemperaturas } from "./api";

async function carregarTemperaturas() {
  try {

    const data = await buscarTemperaturas();


    const temperatura = data.feeds[0].field3;
    const umidade = data.feeds[0].field1;
    const date = new Date(data.feeds[0].created_at).toLocaleString('pt-BR');

    document.getElementById('temperatura').textContent = `Última atualização: ${date}`;

    console.log(temperatura);
    console.log(umidade);
    if (!isNaN(temperatura)) {
      gauge1.refresh(temperatura);
    }

    if (!isNaN(umidade)) {
      gauge2.refresh(umidade);
    }
  } catch (error) {
    console.error('Erro ao buscar dados:', error);
    document.getElementById('temperatura').textContent = 'Erro ao carregar!';
  }
};


// Atualiza a cada 5s
setInterval(carregarTemperaturas, 5000);