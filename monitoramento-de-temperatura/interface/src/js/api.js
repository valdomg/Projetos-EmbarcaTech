

export async function buscarTemperaturas() {
  const response = await fetch("https://api.thingspeak.com/channels/2922648/feeds.json?api_key=E29GDPTCKLB578Q8&results=1");
  const dados = await response.json();
  return dados;
}