
//buscar temperatura de todas as salas
export async function buscarTemperaturas() {
  const token = localStorage.getItem("token");
  const response = await fetch("http://localhost:3000/api/temperatures",
    {headers: {
       "Content-Type": "application/json",
      "Authorization": `Bearer ${token}`   
    }
  });

  if (!response){
    throw new Error(`Erro na requisição: ${response.status}`);
  }
  const dados = await response.json();
  return dados;
}


//buscar intervalo registrado de temperaturas de todas as salas
export async function TempIntervalo(start, end) {
  const token = localStorage.getItem("token");
  const response = await fetch(`http://localhost:3000/api/temperatures/interval?startDate=${start}&endDate=${end}`,
    {
      headers: {
        "Content-Type": "application/json",
        "Authorization": `Bearer ${token}`
      }
    });

  if (!response) {
    throw new Error("Erro ao buscar temperaturas");
  }
  const dados = await response.json();
  return dados;
}


//buscar temperatura de uma sala especifica
export async function buscarTemperaturaSala(salaId)  {
    const token = localStorage.getItem("token");
    const response = await fetch(`http://localhost:3000/api/room/${salaId}/temperatures`,
    {headers: {
       "Content-Type": "application/json",
      "Authorization": `Bearer ${token}`   
    }
  });

  if (!response){
    throw new Error("Erro ao buscar temperaturas");
  }
  const dados = await response.json();
  return dados;
}

//buscar intervalo registrado de temperatura de uma sala especifica
export async function roomTempInterval(salaId, start, end){
  const token = localStorage.getItem("token");
  const response = await fetch(`http://localhost:3000/api/room/${salaId}/temperatures/?start=${start}&end=${end}`,
    {headers: {
       "Content-Type": "application/json",
      "Authorization": `Bearer ${token}`   
    }
  });

  if (!response){
    throw new Error("Erro ao buscar temperaturas");
  }
  const dados = await response.json();
  return dados;
}


//Listar usuários do sistema
export async function usersSearch() {
  const token = localStorage.getItem("token");
  const response = await fetch(`http://localhost:3000/api/users`,
        {
      headers: {
        "Content-Type": "application/json",
        "Authorization": `Bearer ${token}`
      }
    });

  if (!response) {
    throw new Error("Erro ao buscar usuários");
  }
  const dados = await response.json();
  console.log(dados);
  return dados;
}


//Listar salas
export async function roomsSearch() {
  const token = localStorage.getItem("token");
  const response = await fetch(`http://localhost:3000/api/rooms`,
        {
      headers: {
        "Content-Type": "application/json",
        "Authorization": `Bearer ${token}`
      }
    });

  if (!response) {
    throw new Error("Erro ao buscar temperaturas");
  }
  const dados = await response.json();
  console.log(dados);
  return dados;
}