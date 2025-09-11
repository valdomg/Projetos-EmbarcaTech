

export async function buscarTemperaturas() {
  const response = await fetch("http://localhost:3000/api/temperatures");
  const dados = await response.json();
  return dados;
}

export async function buscarTemperaturaSala(salaId)  {
    const response = await fetch(`http://localhost:3000/api/room/${salaId}/temperatures`);
    const dados = await response.json();
    return dados;
}

export async function buscarTempIntervalo(salaId, start, end){
  const response = await fetch(`http://localhost:3000/api/room/${salaId}/temperatures/?start=${start}&end=${end}`);
  const dados = await response.json();
  return dados;
}