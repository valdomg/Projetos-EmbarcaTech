

async function carregarTemperaturas() {
  try {
    const response = await fetch("https://api.thingspeak.com/channels/2922648/feeds.json?api_key=E29GDPTCKLB578Q8&results=1");
    const data = await response.json();


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




var gauge1 = new JustGage({
  id: "gauge", // the id of the html element
  value: 50,
  min: 0,
  max: 100,
  decimals: 0,
  symbol: '°c',
  gaugeWidthScale: 0.9,
  title: "Lone Ranger",
  label: "Última",
  // label: "Temperatura",
  pointer: "true",
  pointerOptions: {
    toplength: 0,
    bottomlength: 15,
    bottomwidth: 3,
    stroke: 'none',
    stroke_width: 0,
    stroke_linecap: 'square',
    color: 'dark'
  }
});

// // update the value randomly
// setInterval(() => {
//   gauge.refresh(Math.random() * 100);
// }, 5000)

var gauge2 = new JustGage({
  id: "gauge2", // the id of the html element
  value: 50,
  min: 0,
  max: 100,
  decimals: 0,
  gaugeWidthScale: 0.9,
  symbol: '%',
  // label: "Umidade",
  pointer: "true",
  pointerOptions: {
    toplength: 0,
    bottomlength: 15,
    bottomwidth: 3,
    stroke: 'none',
    stroke_width: 0,
    stroke_linecap: 'square',
    color: 'dark'
  },
  levelColors: ["#9bd4faff", "#008cffff", "#0400ffff"]
});

// // update the value randomly
// setInterval(() => {
//   gauge2.refresh(Math.random() * 100);
// }, 5000)


document.getElementById('emitirRelatorio').addEventListener('click',
  async () => {
    const dias = parseInt(document.getElementById('periodo').value);

    const dataFim = new Date(); // agora
    const dataInicio = new Date();
    dataInicio.setDate(dataFim.getDate() - dias);

    const start = dataInicio.toISOString(); // formato UTC
    const end = dataFim.toISOString();

    const url = `https://api.thingspeak.com/channels/2922648/feeds.json?api_key=E29GDPTCKLB578Q8&start=${start}&end=${end}`;

    try {
      const response = await fetch(url);
      const data = await response.json();

      const feeds = data.feeds;

      if (!feeds.length) {
        alert("Sem dados para este período.");
        return;
      }

      const csvHeader = 'Data;Temperatura;Umidade\n';
      const csvRows = feeds.map(feed => {
        const data = new Date(feed.created_at).toLocaleString();
        const temp = feed.field3 || '';
        const umid = feed.field2 || '';
        return `${data};${temp};${umid}`;
      });

      const csvContent = csvHeader + csvRows.join('\n');
      downloadCSV(csvContent, `relatorio_${dias}dias.csv`);

    } catch (error) {
      console.error('Erro ao gerar relatório:', error);
      alert("Erro ao gerar relatório.");
    }
  });
  

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