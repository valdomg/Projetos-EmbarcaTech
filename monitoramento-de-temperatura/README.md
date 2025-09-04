# Monitoramento de Temperatura em Salas Hospitalares

O sistema proposto visa automatizar o monitoramento de temperatura em ambientes hospitalares, como salas laboratoriais e áreas de armazenamento de medicamentos. Nesses locais, a manutenção de condições térmicas adequadas pode assegurar a qualidade e a segurança dos insumos, além de garantir a conformidade com os parâmetros estabelecidos pela vigilância sanitária.


---

## 📟 Display LCD 1602 com Interface I2C – Exibição de Temperatura/Umidade com Alertas

### 📖 Visão Geral
Implementa funções para controle do **`display LCD 1602`** via interface **`I2C`**, permitindo a exibição de dados de **_temperatura_** e **_umidade_** com sistema de alertas visuais quando os valores ultrapassam limites pré-definidos.


### 📋 Funcionalidades

✅ Inicialização do display LCD 1602 via I2C.  
✅ Exibição de valores de temperatura e umidade.  
✅ Mensagens de alerta quando valores estão fora do limite.   
✅ Compatível com sensores de temperatura e umidade.  


### 📁 Estrutura dos Arquivos

```
📦 microcontrolador
 ...
 ┣ 📜 display_LCD-1602_I2C.h    # Cabeçalho com declarações das funções
 ┣ 📜 display_LCD-1602_I2C.cpp  # Implementação das funções
```


### ⚙️ Pré-requisitos
#### Bibliotecas Necessárias

- [`Wire.h`](https://www.arduino.cc/en/reference/wire) – Comunicação I2C.
- [`LiquidCrystal_I2C.h`](https://github.com/johnrickman/LiquidCrystal_I2C) – Controle do display LCD via módulo I2C.
- `math.h` - Funções matemáticas (`fabs` para valores absolutos). 

**Instalação na IDE Arduino:**

1. Abra **Rascunho → Incluir Biblioteca → Gerenciar Bibliotecas...**
2. Busque por `LiquidCrystal I2C` e instale a biblioteca
3. A biblioteca `Wire` e `math` já está incluída por padrão na IDE.


### 🚀 Como Usar (Exemplo)

#### 1️⃣ Inclusão no Código Principal (`.ino`)

```cpp
#include "display_LCD-1602_I2C.h"

void setup() {
    lcd1602_init(); // Inicializa o display
}

void loop() {
    // Exemplo com valores simulados
    float temperature = 25.34;
    float humidity = 60.12;
    bool alertTemp = false;
    bool alertHumi = true;

    // Atualiza display com os valores
    lcd1602_showData(
    	temperatura, // temperatura
    	umidade, 	 // umidade
    	alertaTemp,  // alerta temperatura
    	alertaUmid	 // alerta umidade
    	);

    delay(1000); // Atualiza a cada 1 segundo
}
```

#### 2️⃣️ Exemplo no simulador
-    [![Wokwi](https://img.shields.io/badge/Wokwi-161616?style=flat&logo=wokwi&logoColor=white&logoWidth=24)](https://wokwi.com/projects/437135069839959041)


### 📌 Funções Disponíveis - Explicação

#### `lcd1602_init()`
Inicializa o display LCD e exibe uma mensagem "Iniciando...".

#### `lcd1602_showData(float temp, float humi, bool alertTemp, bool alertHumi)`
Exibe temperatura e umidade no display.  

**Parâmetros:**
- `temp`: valor da temperatura em °C.  
- `humi`: valor da umidade em %.  
- `alertTemp`: Flag para ativar alerta de temperatura (`true` ou `false`).  
- `alertHumi`: Flag para ativar alerta de umidade (`true` ou `false`).  


### ⚠️ Observações

1. **Endereço I2C**: O endereço I2C padrão utilizado é `0x27`. Verifique o endereço do seu módulo e atualize a constante `I2C_ADDR` no arquivo **display_LCD-1602_I2C.cpp** conforme necessário.

2. **Linhas e Colunas**: Para usar por exemplo o **display LCD2004 I2C**, basta ajustar as constantes `LCD_COLUMNS` e `LCD_LINES` no arquivo **display_LCD-1602_I2C.cpp**. 
    
    **LCD1602 - 16 colunas × 2 linhas (Utilizado):**
    ```
    	const uint8_t LCD_COLUMNS = 16; // Número de colunas
		const uint8_t LCD_LINES = 2;	// Número de linhas
    ```

    **LCD2004 - 20 colunas × 4 linhas:**
    ```
    	const uint8_t LCD_COLUMNS = 20; // Número de colunas
		const uint8_t LCD_LINES = 4;	// Número de linhas
    ```

3. **Conexões I2C**: 
   ```
	   [MCU SDA] ----> [LCD SDA]
	   [MCU SCL] ----> [LCD SCL]
	   [MCU GND] ----> [LCD GND]
	   [MCU 5V]  ----> [LCD VCC]
   ```
   
4. **Integração com Sensores**: Este módulo apenas exibe dados. A leitura dos sensores deve ser implementada separadamente.

5. **Pinos I2C (SDA/SCL)**: A biblioteca `LiquidCrystal_I2C` simplifica a configuração, **não exigindo definição manual de pinos GPIO no código**. Entretanto, o display deve ser conectado aos `pinos I2C (SDA e SCL)` do microcontrolador.

---


## Protótipo da Página Web
Para acessar o protótipo da página web de monitoramento de temperatura desenvolvido no Figma, [clique aqui.](https://www.figma.com/proto/6rm9JT50d8HA8N9mcLrX5p/MonitoramentoTemperatura?node-id=0-1&t=dfuWBBxjpBx4qWLy-1) 


## Tecnologias utilizadas
![Figma](https://img.shields.io/badge/figma-%23F24E1E.svg?style=for-the-badge&logo=figma&logoColor=white)


