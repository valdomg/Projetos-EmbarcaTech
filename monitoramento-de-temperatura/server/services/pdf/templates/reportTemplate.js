export default function reportTemplate({ roomId, startDate, endDate, readings }) {
  const rows = readings.readings.map(r => `
    <tr>
      <td>${r._id.day}/${r._id.month}/${r._id.year}</td>
      <td>${r._id.hour}:00</td>
      <td>${r.averageTemperature.toFixed(2)} °C</td>
      <td>${r.minTemperature.toFixed(2)} °C</td>
      <td>${r.maxTemperature.toFixed(2)} °C</td>
      <td>${r.averageHumidity.toFixed(2)} %</td>
    </tr>
  `).join('');

  return `
    <html>
      <head>
        <style>
          body { font-family: Arial; padding: 24px; }
          h1 { text-align: center; }
          table { width: 100%; border-collapse: collapse; margin-top: 24px; }
          th, td { border: 1px solid #ddd; padding: 8px; text-align: center; }
          th { background-color: #f4f4f4; }
        </style>
      </head>

      <body>
        <h1>Relatório de Temperatura</h1>

        <p><strong>Sala:</strong> ${roomId}</p>
        <p><strong>Período:</strong> ${startDate} até ${endDate}</p>

        <table>
          <thead>
            <tr>
              <th>Data</th>
              <th>Hora</th>
              <th>Média Temp</th>
              <th>Mín Temp</th>
              <th>Máx Temp</th>
              <th>Média Umidade</th>
            </tr>
          </thead>
          <tbody>
            ${rows}
          </tbody>
        </table>
      </body>
    </html>
  `;
};