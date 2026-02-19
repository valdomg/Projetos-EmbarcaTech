import nodemailer from 'nodemailer';
import dotenv from 'dotenv';
dotenv.config();

export async function sendEnvironmentAlertEmail({
    to,
    roomName,
    temperature,
    humidity,
    minTemp,
    maxTemp,
    minHumidity,
    maxHumidity
}) {
    const html = `
  <html>
  <body style="font-family: Arial, sans-serif; background-color: #f7f9fb; color: #333; padding: 20px;">
    <div style="max-width:600px;background-color:#fff;margin:0 auto;border-radius:12px;
      box-shadow:0 2px 8px rgba(0,0,0,0.1);overflow:hidden;">
      <div style="background-color:#d9534f;color:white;text-align:center;padding:20px;">
        <h2>⚠️ Alerta de Ambiente Crítico</h2>
      </div>
      <div style="padding:20px;">
        <p>Olá,</p>
        <p>O sistema de monitoramento detectou uma <strong>anomalia</strong> na sala 
        <strong>${roomName}</strong>.</p>

        <p style="font-size:1.2em;font-weight:bold;color:#d9534f;">
          Temperatura: ${temperature}°C
        </p>
        <p style="font-size:1.2em;font-weight:bold;color:#d9534f;">
          Umidade: ${humidity}%
        </p>

        <div style="background-color:#f2f2f2;padding:10px;border-radius:8px;margin-top:10px;font-size:0.95em;">
          <p><strong>Limites permitidos:</strong></p>
          <p>Temperatura: ${minTemp}°C - ${maxTemp}°C</p>
          <p>Umidade: ${minHumidity}% - ${maxHumidity}%</p>
        </div>

        <p>Por favor, verifique imediatamente as condições da sala e o funcionamento dos equipamentos de controle climático.</p>

      </div>
      <div style="text-align:center;font-size:0.8em;color:#888;margin-top:20px;padding-bottom:10px;">
        <p>Este é um email automático do Sistema de Monitoramento.</p>
      </div>
    </div>
  </body>
  </html>
  `;

    const transporter = nodemailer.createTransport({
        host: process.env.EMAIL_HOST,
        port: 465,
        secure: true,
        auth: {
            user: process.env.EMAIL_USER,
            pass: process.env.EMAIL_PASS
        }
    });

    const mailOptions = {
        from: `"Monitoramento Hospitalar" <${process.env.EMAIL_USER}>`,
        to,
        subject: `⚠️ Alerta: Temperatura/Umidade crítica na ${roomName}`,
        html,
    };
    await transporter.sendMail(mailOptions);

    console.log(`✅ Email de alerta enviado para ${to}`);
}