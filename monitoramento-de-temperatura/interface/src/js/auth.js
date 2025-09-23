
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
    alert("Sessão expirada, faça login novamente.");
    window.location.href = "login.html";
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
      alert("Sessão expirada, faça login novamente.");
      localStorage.removeItem("token");
      window.location.href = "login.html";
      return false;
    }

    return true; // acesso liberado

  } catch (e) {
    console.error("Erro ao decodificar token:", e);
    alert(`Erro: ${e}`);
    localStorage.removeItem("token");
    window.location.href = "login.html";
    return false;
  }
}