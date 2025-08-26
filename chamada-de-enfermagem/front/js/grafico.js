// documentacao:
// https://www.chartjs.org/docs/latest/developers/api.html

const xValues = ["00:00", "01:00", "02:00", "03:00", "04:00", "05:00", "06:00", "07:00", 
 "08:00", "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", 
 "16:00", "17:00", "18:00", "19:00", "20:00", "21:00", "22:00", "23:00"];
const yValues = [0, 0, 4, 2, 2, 0, 5, 6, ];

grafico = new Chart("grafico-chamadas", {
  type: "line",
  data: {
    labels: xValues,
    datasets: [{
      fill: false,
      lineTension: 0,
      backgroundColor: " rgba(255, 0, 0, 1.0)",
      borderColor: "rgba(0, 0, 0, 0.66)",
      data: yValues
    }]
  },
  options: {
    legend: {display: false},
    scales: {
      yAxes: [{ticks: {min: 0, max:10}}],
    }
  }
});

function atualizarGrafico(hora, quantChamadas) {
    if (
    typeof hora !== 'number' || !Number.isInteger(hora) ||
    typeof quantChamadas !== 'number' || !Number.isInteger(quantChamadas)
  ) {
    throw new Error("Ambos os parâmetros devem ser inteiros");
  }
    grafico.data.datasets[0].data[hora] = quantChamadas;
    grafico.update();
}