import puppeteer from 'puppeteer';
import reportTemplate from './templates/reportTemplate.js';
import fs from 'fs';
import path from 'path';

export class ReportPdfService {

  static async generate({ startDate, endDate, readings }) {
    const html = reportTemplate({ roomId: readings.room.name, startDate, endDate, readings });

    const reportsDir = path.join(process.cwd(), 'public', 'reports');

    if (!fs.existsSync(reportsDir)) {
      fs.mkdirSync(reportsDir, { recursive: true });
    }

    const fileName = `Relatorio-${readings.room.name.replace(/ /g, "-")}-${startDate}-${endDate}-${Date.now()}.pdf`;
    const outputPath = path.join(reportsDir, fileName);
 
    const browser = await puppeteer.launch();
    const page = await browser.newPage();

    await page.setContent(html, { waitUntil: 'networkidle0' });

    await page.pdf({
      path: outputPath,
      format: 'A4',
      printBackground: true
    });

    await browser.close();

    return {
      fileName,
      filePath: outputPath,
      publicUrl: `/reports/${fileName}`
    };
  }
}