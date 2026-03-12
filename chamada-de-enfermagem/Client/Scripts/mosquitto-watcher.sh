#!/bin/bash

# Caminhos (Ajuste conforme sua instalação)
PASSWD_FILE="/etc/mosquitto/passwd"
PID_FILE="/var/run/mosquitto/mosquitto.pid"

echo "Monitorando alterações em $PASSWD_FILE..."

# Loop infinito usando inotifywait
while inotifywait -e close_write "$PASSWD_FILE"; do
    echo "Alteração detectada! Recarregando configurações do Mosquitto..."
    
    # Verifica se o processo está rodando e envia o sinal SIGHUP
    if [ -f "$PID_FILE" ]; then
        kill -HUP $(cat "$PID_FILE")
    else
        # Alternativa caso não use PID file: kill -HUP $(pidof mosquitto)
        pidof mosquitto > /dev/null && kill -HUP $(pidof mosquitto)
    fi
    
    echo "Sinal SIGHUP enviado com sucesso."
done
