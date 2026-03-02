
import { userLogin } from "./api.js";


//login 
const form = document.getElementById("loginForm");

if (form) {
    form.addEventListener('submit', async function (event) {
        event.preventDefault(); // Impede o envio tradicional do formulário

        const email = document.getElementById("email").value;
        const senha = document.getElementById("senha").value;

        try {
            const result = await userLogin(email, senha);
            console.log("Usuário cadastrado:", result);
        } catch (err) {
            console.error("Erro:", err);
        }
    });
}


//verificar acesso e permissoes
export function checkAcess(permissao = null) {
  const token = localStorage.getItem("token");

  if (!token) {
    // alert("Sessão expirada, faça login novamente.");
    window.location.href = "login.html";
    const alerta = document.getElementById("responseUser");
    alerta.innerText = "Sessão expirada, faça login novamente.";
    return false;
  }

  try {
    const decoded = jwt_decode(token);
    const now = Date.now() / 1000;
    console.log("Token decodificado:", decoded);


    if (permissao && decoded.role !== permissao) {
      alert("Acesso negado! Apenas " + permissao + "s podem acessar esta página.");
      window.location.href = "index.html";
      return false;
    }

    if (decoded.exp && decoded.exp < now) {
      // alert("Sessão expirada, faça login novamente.");
      localStorage.removeItem("token");
      window.location.href = "login.html";
      // console.error("Erro ao fazer login:", error);
      const alerta = document.getElementById("responseUser");
      alerta.innerText = "Sessão expirada, faça login novamente.";
      return false;
    }

    document.body.classList.remove("hide");
    document.body.classList.add("show");
    return true; // acesso liberado

  } catch (e) {
    console.error("Erro ao decodificar token:", e);
    const alerta = document.getElementById("responseUser");
    // alert(`Erro: ${e}`);
    localStorage.removeItem("token");
    window.location.href = "login.html";
    alerta.innerText = `${e}`;
    return false;
  }
}


//função abrir e fechar modal
export function abrirModal(modalId, closeBtnSelector) {
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

//Função logout usuário
export function logoutUser() {
  // Confirmar exclusão
  // Mostrar o modal
  const modal = document.getElementById('modalLogout');
  abrirModal('modalLogout', '#logoutNo');

  // Mensagem final
  const mensagem = "Tem certeza que deseja sair?";
  document.getElementById('logoutMessage').innerText = mensagem;

  const logout = document.getElementById("logoutYes");
  logout.onclick = async (e) => {
    e.preventDefault();

    modal.close();
    localStorage.removeItem("token");
    window.location.href = "login.html";
  }
}


