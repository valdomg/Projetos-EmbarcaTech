import EmailService from "./sendEmail.js";
import '../../database/db.js';

const service = new EmailService();

service.sendAlertEmail("mcu1234", 28, 40).then(() => {
    console.log("Alert email sent successfully.");
}).catch((error) => {
    console.error("Error sending alert email:", error);
});