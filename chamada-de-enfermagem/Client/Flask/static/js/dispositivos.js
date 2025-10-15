const abrir = document.getElementById('abrir');
const fechar = document.getElementById('fechar');
const form = document.getElementById('formulario');
const formUsers = document.getElementById('formUser');

abrir.addEventListener('click', () => {
  form.style.display = 'block';
});

fechar.addEventListener('click', () => {
  form.style.display = 'none';
});

formUsers.addEventListener('submit', async (e) => {
  e.preventDefault();

  const formData = new FormData(formUsers);
  const dados = Object.fromEntries(formData.entries());

  try {
    const resposta = await fetch('/api/devices/register', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'  
      },
      body: JSON.stringify(dados)
    });

    const resultado = await resposta.json();
    console.log(resultado);
    carregarUsuarios();

  } catch (error) {
    console.error(error);
  }
});

async function carregarUsuarios() {
  const resposta = await fetch('/api/devices');
  const devices = await resposta.json();

  const planilha = document.getElementById('userTable');

  planilha.innerHTML = "";

  devices.forEach(device => {
    const linha = document.createElement('div');
    linha.classList.add('linha3');

    linha.innerHTML += `
      <div class="celula">${device._id}</div>
      <div class="celula">${device.device}</div>
      <div class="celula">
        <button class="btn-excluir" data-id="${device._id}">Excluir</button>
        <button class="btn-editar" data-id="${device._id}">Alterar</button>
      </div>
    `

    planilha.appendChild(linha);
    })

  document.querySelectorAll('.btn-excluir').forEach(btn => {
    btn.addEventListener('click', async function() {
      const id = this.getAttribute('data-id');
      const resposta = await fetch(`/api/devices/delete/${id}`, { method: 'DELETE' });
      if (resposta.ok) {
        alert(`Usuário ${id} excluído`);
        location.reload(); 
      } else {
        alert(`Erro ao excluir o usuário ${id}`);
      }
    });
  });
}

carregarUsuarios();
