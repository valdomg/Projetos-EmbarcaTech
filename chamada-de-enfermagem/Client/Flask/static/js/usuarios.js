const abrir = document.getElementById('abrir');
const fechar = document.getElementById('fechar');
const form = document.getElementById('formulario');

abrir.addEventListener('click', () => {
  form.style.display = 'block';
});

fechar.addEventListener('click', () => {
  form.style.display = 'none';
});

async function carregarUsuarios() {
  const resposta = await fetch('/usuarios');  // pega JSON do Flask
  const usuarios = await resposta.json();

  const planilha = document.getElementById('planilha');

  usuarios.forEach(usuario => {
    planilha.innerHTML += `
      <div class="celula">${usuario.id}</div>
      <div class="celula">${usuario.nome}</div>
      <div class="celula">${usuario.funcao}</div>
      <div class="celula">
        <button class="btn-excluir" data-id="${usuario.id}">Excluir</button>
        <button class="btn-editar" data-id="${usuario.id}">Alterar</button>
      </div>
    `
    })

  document.querySelectorAll('.btn-excluir').forEach(btn => {
    btn.addEventListener('click', async function() {
      const id = this.getAttribute('data-id');
      const resposta = await fetch(`/usuarios/excluir/${id}`, { method: 'DELETE' });
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
