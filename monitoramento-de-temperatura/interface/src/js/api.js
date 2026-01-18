
//--------------------TEMPERATURAS----------------
//buscar temperatura de todas as salas
export async function buscarTemperaturas() {
  const token = localStorage.getItem("token");
  const response = await fetch("http://localhost:3000/api/temperatures",
    {
      headers: {
        "Content-Type": "application/json",
        "Authorization": `Bearer ${token}`
      }
    });

  if (!response) {
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


//---------------SALAS-------------------------
//buscar temperatura de uma sala especifica
export async function buscarTemperaturaSala(salaId) {
  const token = localStorage.getItem("token");
  const response = await fetch(`http://localhost:3000/api/room/${salaId}/temperatures`,
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

//buscar intervalo registrado de temperatura de uma sala especifica
export async function roomTempInterval(salaId, start, end) {

  const token = localStorage.getItem("token");
  const response = await fetch(`http://localhost:3000/api/room/${salaId}/temperatures/interval/?startDate=${start}&endDate=${end}`,
    {
      headers: {
        "Content-Type": "application/json",
        "Authorization": `Bearer ${token}`
      }
    });

  const dados = await response.json();

  if (!response) {
    throw new Error(dados.erro || dados.message || "Erro ao buscar temperaturas");
  }

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
    throw new Error("Erro ao buscar salas");
  }
  const dados = await response.json();
  console.log(dados);
  return dados;
}

//Cadastrar sala
export async function roomRegister(nome, microcontrolador) {

  const token = localStorage.getItem("token");
  const body = {
    name: nome,
    microcontrollerId: microcontrolador
  };
  try {
    const response = await fetch(`http://localhost:3000/api/room`,
      {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
          "Authorization": `Bearer ${token}`
        },
        body: JSON.stringify(body),
      });


    if (!response.ok) {
      const erroData = await response.json();
      console.log(erroData);
      throw new Error(erroData.message || "Erro ao cadastrar ambiente");
    }

    const data = await response.json();
    return data;

  } catch (err) {
    console.error("Falha no cadastro:", err.message);
    throw err;
  }
}


//Editar sala
export async function roomEdit(id, nome, microcontrolador) {

  const token = localStorage.getItem("token");
  const body = {
    id: id,
    name: nome,
    microcontrollerId: microcontrolador
  };
  try {
    const response = await fetch(`http://localhost:3000/api/room/${id}`,
      {
        method: "PUT",
        headers: {
          "Content-Type": "application/json",
          "Authorization": `Bearer ${token}`
        },
        body: JSON.stringify(body),
      });


    if (!response.ok) {
      const erroData = await response.json();
      console.log(erroData);
      throw new Error(erroData.message || "Erro ao editar ambiente");
    }

    const data = await response.json();
    return data;

  } catch (err) {
    console.error("Falha no cadastro:", err.message);
    throw err;
  }
}


//Deletar sala
export async function roomDelete(roomId) {
  const token = localStorage.getItem("token");

  const response = await fetch(`http://localhost:3000/api/room/${roomId}`, {
    method: "DELETE",
    headers: {
      "Content-Type": "application/json",
      "Authorization": `Bearer ${token}`,
    },
  });


  if (response.status === 204) {
    return { ok: true };
  }

  let dados = {};
  try {
    dados = await response.json();
  } catch (_) {

  }

  return {
    ok: response.ok,
    ...dados,
  };
}



//----------------------USUARIOS--------------------
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

//Login de usuario
export async function userLogin(email, senha) {

  const body = {
    email: email,
    password: senha
  };

  try {
    // Faz a requisição de login
    const response = await fetch("http://localhost:3000/api/login", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(body),
    });

    const data = await response.json();
    console.log(data);

    if (response.ok) {
      localStorage.setItem("token", data.token);

      const decode = jwt_decode(data.token);

      if (decode.role == "admin") {
        window.location.href = "admin.html";
      } else {
        window.location.href = "index.html";
      }


    } else {
      const alerta = document.getElementById("responseUser");
      alerta.innerText = `${data.message || data.erro || "Usuário ou senha inválida."}`;
      // setTimeout(() => { alerta.innerText = ""; }, 3000);

    }
  } catch (error) {
    console.error("Erro ao fazer login:", error);

    if (error instanceof TypeError && error.message === "Failed to fetch") {
      const alerta = document.getElementById("responseUser");
      alerta.innerText = "Erro de conexão com o servidor. Tente novamente.";
    } else {
      const alerta = document.getElementById("responseUser");
      alerta.innerText = "Ocorreu um erro inesperado.";
    }
  }
}


//Cadastrar usuarios no sistema
export async function userRegister(usuario, email, senha) {

  const token = localStorage.getItem("token");
  const body = {
    name: usuario,
    email: email,
    password: senha
  };
  try {
    const response = await fetch(`http://localhost:3000/api/user/`,
      {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
          "Authorization": `Bearer ${token}`
        },
        body: JSON.stringify(body),
      });

    const data = await response.json();

    if (!response.ok) {
      console.log(data);
      throw new Error(data.erro || data.message || "Erro ao cadastrar usuário");
    }


    return data;

  } catch (err) {
    console.error("Falha no cadastro:", err.message);
    throw err;
  }
}


//Editar dados do usuario
export async function userEdit(id, usuario, email, senha) {

  const token = localStorage.getItem("token");
  const body = {
    name: usuario,
    email: email,
    password: senha,
    userId: id
  };
  try {
    const response = await fetch(`http://localhost:3000/api/user/${id}`,
      {
        method: "PUT",
        headers: {
          "Content-Type": "application/json",
          "Authorization": `Bearer ${token}`
        },
        body: JSON.stringify(body),
      });


    if (!response.ok) {
      const erroData = await response.json();
      throw new Error(erroData.message || "Erro ao editar usuário");
    }

    const data = await response.json();
    return data;

  } catch (err) {
    console.error("Falha na edição:", err.message);
    throw err;
  }
}


//Deletar usuário 
export async function userDelete(userId) {
  const token = localStorage.getItem("token");

  const response = await fetch(`http://localhost:3000/api/user/${userId}`, {
    method: "DELETE",
    headers: {
      "Content-Type": "application/json",
      "Authorization": `Bearer ${token}`,
    },
  });


  if (response.status === 204) {
    return { ok: true };
  }

  let dados = {};
  try {
    dados = await response.json();
  } catch (_) {

  }

  return {
    ok: response.ok,
    ...dados,
  };
}