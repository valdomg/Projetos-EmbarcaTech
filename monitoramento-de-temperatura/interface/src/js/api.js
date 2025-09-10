

export async function buscarTemperaturas() {
  const response = await fetch("http://localhost:3000/api/temperatures");
  const dados = await response.json();
  return dados;
}