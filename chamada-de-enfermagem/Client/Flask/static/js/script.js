const socket = io();

const chamadasLista = document.getElementById("chamadas-lista");

function adicionarChamada(chamada) {
    const div = document.createElement("div");
    div.className = "row border rounded mb-3";
    div.textContent = `Chamada: Enfermaria ${chamada.enfermaria}; ${chamada.horario}.`;
    chamadasLista.prepend(div); 
}


socket.on("todas-chamadas", (chamadas) => {
    chamadas.forEach(chamada => adicionarChamada(chamada));
});

socket.on("nova-chamada", (chamada) => {
    adicionarChamada(chamada);
});
