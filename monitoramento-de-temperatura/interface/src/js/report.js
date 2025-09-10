
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