const abrir = document.getElementById('abrir');
const fechar = document.getElementById('fechar');
const fecharPut = document.getElementById('fecharPut');
const form = document.getElementById('formCadastro');
const formPut = document.getElementById('formPut');
const overlay = document.getElementById('overlay');

let idEditando = null;

abrir.addEventListener('click', () => {
  form.style.display = 'block';
  overlay.style.display = 'block';
});

fechar.addEventListener('click', () => {
  form.style.display = 'none';
  overlay.style.display = 'none';
});

fecharPut.addEventListener('click', () => {
  overlay.style.display = 'none';
  formPut.style.display = 'none';
});

form.addEventListener('submit', async (e) => {
  e.preventDefault();

  const formData = new FormData(form);
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
    carregarDispositivos();

  } catch (error) {
    console.error(error);
  }
});

formPut.addEventListener('submit', async (e) => {
  e.preventDefault();

  const formData = new FormData(formPut);
  const dados = Object.fromEntries(formData.entries());

  dados["document_id"] = idEditando;

  try {
      // Modo edição
      const resposta = await fetch(`/api/devices/update`, {
        method: 'PUT',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(dados)
      });

      if (resposta.ok) {
        formPut.style.display = 'none';
        overlay.style.display = 'none';
        carregarDispositivos();
      } else {
        alert('Erro ao atualizar usuário.');
      } 
  } catch (error) {
    console.error(error);
  }
});

async function carregarDispositivos() {
  const resposta = await fetch('/api/devices');
  const devices = await resposta.json();

  const planilha = document.getElementById('userTable');

  planilha.innerHTML = "";

  devices.forEach(device => {
    const linha = document.createElement('div');
    linha.classList.add('linha3');

    linha.innerHTML += `
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
        location.reload(); 
      } else {
        alert(`Erro ao excluir o usuário ${id}`);
      }
    });
  },

  document.querySelectorAll('.btn-editar').forEach(btn => {
    btn.addEventListener('click', function () {
      const id = this.getAttribute('data-id');
      const linha = this.closest('.linha3');
      const nome = linha.querySelector('.celula:nth-child(1)').innerText;

      document.querySelector('#formPut input[name="device"]').value = nome;

      idEditando = id;

      formPut.style.display = 'block';
      overlay.style.display = 'block';
    });
  })
);

}

carregarDispositivos();
