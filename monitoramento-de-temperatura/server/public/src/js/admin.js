import { checkAcess } from './auth.js';
import { roomsSearch, usersSearch, userRegister, userEdit, userDelete, roomDelete, roomRegister, roomEdit } from './api.js';
import { carregarTemperaturas } from './main.js'
import { gerarRelatorio } from './report.js'

// --- verifica permissões ---
document.addEventListener("DOMContentLoaded", () => {
  checkAcess("admin");
});


// função para renderizar tabela de dispositivos e ambientes
async function renderSalas() {

  const dados = await roomsSearch();

  if (!dados.length) {
    console.error("Nenhum dado encontrado.");
    return;
  }

  const grid = document.getElementById("dashboard");
  grid.innerHTML = "";

  const table = document.createElement("table");

  table.innerHTML = `
                            <caption>Salas</caption>
                            <thead>
                                <tr>
                                <th scope="col">Ambiente</th>
                                <th scope="col">Microcontrolador</th>
                                <th scope="col">ID</th>
                                <th scope="col">Ações</th>
                            </tr>
                            </thead>
                            <tbody></tbody>
                            `;

  const tbody = table.querySelector("tbody");


  dados.forEach(sala => {
    const row = document.createElement("tr");
    row.setAttribute("data-id", sala._id);
    row.innerHTML = `
                            <td>${sala.name.toUpperCase()}</td>
                            <td>${sala.microcontrollerId}</td>
                            <td>${sala._id}</td>
                            <td>
                            <button data-id="${sala._id}" class="btn-warning editarSala" >Editar</button>
                            <button data-id="${sala._id}" class="btn excluirSala">Excluir</button>
                            </td>
                            `;
    tbody.appendChild(row);
  });

  grid.appendChild(table);

  const button = document.getElementById('btn-click');

  button.classList.remove('hide');
  button.classList.add('show');

  button.classList.remove('userInsert');
  button.classList.add('roomInsert')
}


// função para renderizar tabela de usuários
async function renderUsuarios() {
  const dados = await usersSearch();

  if (!dados.length) {
    console.error("Nenhum dado encontrado.");
    return;
  }
  const grid = document.getElementById("dashboard");
  grid.innerHTML = "";

  const table = document.createElement("table");

  table.innerHTML = `
                            <caption>Usuários</caption>
                            <thead>
                                <tr>
                                    <th scope="col">Usuário</th>
                                    <th scope="col">Email</th>
                                    <th scope="col">ID</th>
                                    <th scope="col">Ações</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        `;

  const tbody = table.querySelector("tbody");


  dados.forEach(sala => {
    const row = document.createElement("tr");
    row.setAttribute("data-id", sala._id);
    row.innerHTML = `
          <td>${sala.name.toUpperCase()}</td>
          <td>${sala.email}</td>
          <td>${sala._id}</td>
          <td>
          <button data-id="${sala._id}" class="btn-warning editarUsuario" >Editar</button>
          <button data-id="${sala._id}" class="btn excluirUsuario">Excluir</button>
        </td>
        `;
    tbody.appendChild(row);
  });
  grid.appendChild(table);

  // Alterar a classe do botão para mostrar o conteúdo
  const button = document.getElementById('btn-click');

  button.classList.remove('hide');
  button.classList.add('show');

  button.classList.remove('roomInsert');
  button.classList.add('userInsert')
}

// carregarTemperaturas();
const links = document.querySelectorAll('.menu-lateral a[data-section]');


links.forEach(link => {

  //eventListener para criar o conteudo da pagina dinamicamente de acordo com a opção do menu lateral do dashboard
  link.addEventListener('click', async (e) => {
    e.preventDefault();

    // Remove os cards de temperatura apos clicar nas opções menu lateral
    const roomsGrid = document.getElementById('roomsGrid');
    if (roomsGrid) {
      roomsGrid.remove();
    }


    const section = link.dataset.section;
    console.log(section);

    if (section === 'room') {
      await renderSalas();

    } else if (section === 'user') {
      await renderUsuarios();

    } else if (section === 'reports') {
      try {
        const dados = await roomsSearch();
        console.log(dados);

        if (!dados.length) {
          console.error(`Nenhum dado encontrado para o período`);
          window.alert("Sem dados para o período");
          return;
        }

        const grid = document.getElementById("dashboard");
        grid.innerHTML = "";

        const relatorio = document.createElement("div");
        relatorio.classList.add("relatorio");
        relatorio.innerHTML = `
                <h3>Emitir Relatório</h3>
                <p id="alertRelatorio" class="alert"></p>
                <div>
                    <label>Escolha o local</label>
                    <select name="room" id="room-select">

                    <option value="" disabled selected>-- Escolha uma sala --</option>

                    </select>
                    <label for="periodo">Escolha o período:</label>
                    <input id="start" class="date" type="date">
                    <label>à </label>
                    <input id="end" class="date" type="date">
                    <button type="button" class="btn emitirRelatorio">Baixar</button>
                </div>
                
                `;

        grid.appendChild(relatorio);

        const select = document.getElementById('room-select');
        dados.forEach(sala => {
          select.innerHTML += `<option value="${sala._id}">${sala.name}</option>`;
        });


        // Alterar a classe do botão para mostrar o conteúdo
        const button = document.getElementById('btn-click');
        button.classList.remove('show');
        button.classList.add('hide');


      } catch (error) {
        console.error("Erro ao carregar sala:", error);

        if (error.message.includes("401") || error.message.includes("403")) {
          alert("Acesso não autorizado. Faça login novamente.");
          localStorage.removeItem("token");
          window.location.href = "login.html";
        }
      }
    }

    else if (section === 'dashboard') {
      location.reload();
    }
  });
});


// Evento para cadastrar
document.addEventListener('click', (e) => {
  if (e.target.classList.contains('userInsert')) {

    // Mostrar o modal
    abrirModal('modalUserInsert', '#userCloseBtn');

    const cadastrar = document.getElementById("userInsertBtn");
    cadastrar.onclick = async () => {

      const usuario = document.getElementById("name").value;
      const email = document.getElementById("email").value;
      const senha = document.getElementById("senha").value;
      const senha2 = document.getElementById("senha2").value;

      if (senha !== senha2) {
        const alerta = document.getElementById("responseUser");
        alerta.innerText = "As senhas devem ser iguais!";

        // Limpa o texto de alerta depois de 3 segundos
        setTimeout(() => { alerta.innerText = ""; }, 7000);
        return;
      }

      try {
        //requisicao da API para cadastrar usuario
        const data = await userRegister(usuario, email, senha);
        console.log(data);

        if (data && data.id) {
          const alerta = document.getElementById("responseUser");
          alerta.innerText = `Usuário ${data.name} cadastrado com sucesso!`;

          // Limpa o texto de alerta depois de 3 segundos
          setTimeout(() => { alerta.innerText = ""; }, 7000);

          //Limpa o formulário
          document.getElementById("userForm").reset();
          await renderUsuarios();
        } else {

          // alerta de erro;
          const alerta = document.getElementById("responseUser");
          alerta.innerText = data.erro || data.message || "Erro desconhecido no cadastro";
          // Limpa o texto depois de 3 segundos
          setTimeout(() => { alerta.innerText = ""; }, 7000);
        }
      } catch (error) {
        alert(`${error.message}`);
      }
    };

  } else if (e.target.classList.contains('roomInsert')) {

    // Mostrar o modal
    abrirModal('modalRoomInsert', '#roomCloseBtn');


    const cadastrar = document.getElementById("roomInsertBtn");
    cadastrar.onclick = async () => {
      const roomName = document.getElementById("roomName").value;
      const microName = document.getElementById("roomMicro").value;

      try {
        //requisicao da API para cadastrar usuario
        const data = await roomRegister(roomName, microName);
        console.log(data);

        if (data && data._id) {
          const alerta = document.getElementById("responseRoom");
          alerta.innerText = `Ambiente ${data.name} cadastrado com sucesso!`;

          // Limpa o texto de alerta depois de 3 segundos
          setTimeout(() => { alerta.innerText = ""; }, 7000);

          //Limpa o formulário
          document.getElementById("roomForm").reset();
          await renderSalas();
        } else {

          // alerta de erro;
          const alerta = document.getElementById("responseRoom");
          alerta.innerText = data.erro || data.message || "Erro desconhecido no cadastro";
          // Limpa o texto depois de 3 segundos
          setTimeout(() => { alerta.innerText = ""; }, 7000);
        }
      } catch (error) {
        alert(`${error.message}`);
      }
    };
  } else if (e.target.classList.contains('emitirRelatorio')) {
    gerarRelatorio();
  }

});



//função abrir e fechar modal
function abrirModal(modalId, closeBtnSelector) {
  // Mostrar o modal
  const modal = document.getElementById(modalId);
  if (!modal) {
    console.log(`Modal com ID "${modalId}" não encontrado.`);
    return;
  }
  modal.showModal();

  //Fechar modal
  const btnClose = modal.querySelector(closeBtnSelector);
  if (!btnClose) {
    console.log(`Modal com ID "${closeBtnSelector}" nao encontrado`)
    return;
  }
  btnClose.onclick = () => modal.close();
}



// // Evento para cadastrar
// document.getElementById('btn-click').addEventListener('click', () => {

//   // Mostrar o modal
//   const modal = (document.getElementById('userInsert'));
//   modal.showModal();

//   //Fechar modal
//   const btn_close = modal.querySelector("#userCloseBtn");
//   btn_close.onclick = function () {
//     modal.close();
//   }

//   const cadastrar = document.getElementById("userInsertBtn");
//   cadastrar.onclick = async () => {
//     const usuario = document.getElementById("name").value;
//     const email = document.getElementById("email").value;
//     const senha = document.getElementById("senha").value;
//     const senha2 = document.getElementById("senha2").value;

//     if (senha !== senha2) {
//         const alerta = document.getElementById("response");
//         alerta.innerText = "As senhas devem ser iguais!";

//         // Limpa o texto de alerta depois de 3 segundos
//         setTimeout(() => {alerta.innerText = "";}, 3000);
//       return;
//     }

//     try {
//       //requisicao da API para cadastrar usuario
//       const data = await userRegister(usuario, email, senha) ;
//       console.log(data);

//       if (data && data.id) {
//         const alerta = document.getElementById("response");
//         alerta.innerText = `Usuário ${data.name} cadastrado com sucesso!`;

//         // Limpa o texto de alerta depois de 3 segundos
//         setTimeout(() => {alerta.innerText = "";}, 3000);

//         //Limpa o formulário
//         document.getElementById("userForm").reset();
//       } else {

//         // alerta de erro;
//         const alerta = document.getElementById("response");
//         alerta.innerText = data.erro || data.message || "Erro desconhecido no cadastro";
//         // Limpa o texto depois de 3 segundos
//         setTimeout(() => {alerta.innerText = "";}, 3000);
//       }
//     } catch (error) {
//       alert(`${error.message}`);
//     }
//   };
// });


// Evento para excluir e editar usuário ou sala
document.addEventListener('click', function (e) {
  if (e.target && e.target.classList.contains('excluirUsuario')) {
    const id = e.target.getAttribute('data-id');
    excluirUsuario(id);

  } else if (e.target && e.target.classList.contains('excluirSala')) {
    const id = e.target.getAttribute('data-id');
    excluirSala(id);
  } else if (e.target && e.target.classList.contains('editarSala')) {
    const id = e.target.getAttribute('data-id');
    editarSala(id);
  } else if (e.target && e.target.classList.contains('editarUsuario')) {
    const id = e.target.getAttribute('data-id');
    editarUsuario(id);
  }
});



// Funcao editar dados da sala
async function editarSala(id) {
  // Mostrar o modal
  abrirModal('modalRoomEdit', '#roomEditCloseBtn');

  const linha = document.querySelector(`tr[data-id='${id}']`);

  const nome = linha.getElementsByTagName('td')[0].innerText;
  const micro = linha.getElementsByTagName('td')[1].innerText;

  const nomeInput = document.getElementById("roomNameEdit");
  const microInput = document.getElementById("roomMicroEdit");

  if (nomeInput && microInput) {
    nomeInput.value = nome;
    microInput.value = micro;
  } else {
    console.error("Campos de entrada não encontrados no modal");
  }

  const editar = document.getElementById("roomEditBtn");
  editar.onclick = async (e) => {
    e.preventDefault();

    try {
      const nome = document.getElementById("roomNameEdit").value;
      const micro = document.getElementById("roomMicroEdit").value;
      //requisicao da API para editar sala
      const data = await roomEdit(id, nome, micro);
      console.log(data);

      if (data && data._id) {
        const alerta = document.getElementById("responseRoomEdit");
        alerta.innerText = `Ambiente ${data.name} editado com sucesso!`;

        // Limpa o texto de alerta depois de 3 segundos
        setTimeout(() => { alerta.innerText = ""; },7000);

        //Limpa o formulário
        document.getElementById("roomFormEdit").reset();
        await renderSalas();
      } else {

        // alerta de erro;
        const alerta = document.getElementById("responseRoomEdit");
        alerta.innerText = data.erro || data.message || "Erro desconhecido na edição";
        // Limpa o texto depois de 3 segundos
        setTimeout(() => { alerta.innerText = ""; }, 7000);
      }
    } catch (error) {
      alert(`${error.message}`);
    }
  };
}


// Funcao editar dados do usuário
async function editarUsuario(id) {
  // Mostrar o modal
  abrirModal('modalUserEdit', '#editCloseBtn');

  const linha = document.querySelector(`tr[data-id='${id}']`);

  const nome = linha.getElementsByTagName('td')[0].innerText;
  const email = linha.getElementsByTagName('td')[1].innerText;

  const nomeInput = document.getElementById("nameEdit");
  const emailInput = document.getElementById("emailEdit");

  if (nomeInput && emailInput) {
    nomeInput.value = nome;
    emailInput.value = email;
  } else {
    console.error("Campos de entrada não encontrados no modal");
  }

  const editar = document.getElementById("userEditBtn");
  editar.onclick = async (e) => {
    e.preventDefault();

    try {
      const nome = document.getElementById("nameEdit").value;
      const email = document.getElementById("emailEdit").value;
      const senha = document.getElementById('senhaEdit').value;
      const senha2 = document.getElementById('senha2Edit').value;
      //requisicao da API para editar sala

      if (senha != senha2) {
        alerta.innerText = data.erro || data.message || "Erro desconhecido";
        return;
      }
      const data = await userEdit(id, nome, email, senha);
      console.log(data);

      if (data && data._id) {
        const alerta = document.getElementById("responseUserEdit");
        alerta.innerText = `Usuário ${data.name} editado com sucesso!`;

        // Limpa o texto de alerta depois de 3 segundos
        setTimeout(() => { alerta.innerText = ""; }, 7000);

        //Limpa o formulário
        document.getElementById("roomFormEdit").reset();
        await renderUsuarios();
      } else {

        // alerta de erro;
        const alerta = document.getElementById("responseRoomEdit");
        alerta.innerText = data.erro || data.message || "Erro desconhecido na edição";
        // Limpa o texto depois de 3 segundos
        setTimeout(() => { alerta.innerText = ""; }, 7000);
      }
    } catch (error) {
      alert(`${error.message}`);
    }
  };
}


//Função excluir usuário
async function excluirUsuario(id, colunaIndex = 0) {
  // Confirmar exclusão
  // Mostrar o modal
  const modal = document.getElementById('modalDelete');
  abrirModal('modalDelete', '#confirmNo');

  // Pegar a linha pelo id
  const linha = document.querySelector(`tr[data-id='${id}']`);

  // Pegar o texto da célula específica
  const valor = linha.getElementsByTagName('td')[colunaIndex].innerText;

  // Pegar o cabeçalho da tabela correspondente
  const tabela = linha.closest('table');
  const th = tabela.querySelectorAll('thead th')[colunaIndex].innerText;

  const mensagem = `Tem certeza que deseja excluir permanentemente "${th}: ${valor}"?`;

  document.getElementById('deleteMessage').innerText = mensagem;

  const excluir = document.getElementById("confirmYes");
  excluir.onclick = async (e) => {
    e.preventDefault();

    try {
      const data = await userDelete(id);


      if (data.ok) {
        removerLinhaTabela(id);
        modal.close();
        return;

      }
      alert(data.erro || data.message || "Erro desconhecido ao excluir o item");

    }
    catch (error) {
      alert(`Erro: ${error.message}`);
    }
  }
}


//Função excluir usuário
async function excluirSala(id, colunaIndex = 0) {
  // Confirmar exclusão
  // Mostrar o modal
  const modal = document.getElementById('modalDelete');
  abrirModal('modalDelete', '#confirmNo');

  // Pegar a linha pelo id
  const linha = document.querySelector(`tr[data-id='${id}']`);

  // Pegar o texto da célula específica
  const valor = linha.getElementsByTagName('td')[colunaIndex].innerText;


  // Pegar o cabeçalho da tabela correspondente
  const tabela = linha.closest('table');
  const th = tabela.querySelectorAll('thead th')[colunaIndex].innerText;


  // Mensagem final
  const mensagem = `Tem certeza que deseja excluir permanentemente "${th}: ${valor}"?`;
  document.getElementById('deleteMessage').innerText = mensagem;

  const excluir = document.getElementById("confirmYes");
  excluir.onclick = async (e) => {
    e.preventDefault();

    try {

      const data = await roomDelete(id);


      if (data.ok) {
        removerLinhaTabela(id);
        modal.close();
        return;

      }
      alert(data.erro || data.message || "Erro desconhecido ao excluir o item");

    }
    catch (error) {
      alert(`Erro: ${error.message}`);
    }
  }
}



// Função para remover a linha da tabela
function removerLinhaTabela(id) {
  const linha = document.querySelector(`tr[data-id='${id}']`);
  if (linha) {
    linha.remove();
  } else {
    console.error("Linha não encontrada!");
  }
}


// //Evento para logout
// document.getElementById('logoutBtn').addEventListener('click', () => {
//   alert('Logout realizado!');
// });