# Projeto: Ohmímetro Colorido: Medindo Resistores com Visual Interativo

## Descrição

Este projeto tem como objetivo modificar o funcionamento de um ohmímetro, utilizando a placa **RP2040** (BitDogLab) para medir a resistência de um resistor e exibir o valor medido, bem como as cores correspondentes a esse valor de acordo com a série **E24**. O sistema exibe o valor da resistência e as cores das faixas de um resistor no display **OLED SSD1306** e também utiliza uma matriz de LEDs para representar as faixas de cor.

## Funcionalidades

- **Leitura da resistência**: O sistema realiza a leitura da resistência de um resistor conectado ao circuito.
- **Cálculo da resistência**: O valor da resistência é calculado utilizando o ADC (Conversor Analógico para Digital) do RP2040.
- **Identificação do valor comercial**: O sistema identifica o valor comercial mais próximo do resistor com base na série **E24** (com tolerância de 5%).
- **Exibição de faixas de cores**: As três primeiras faixas de cores do resistor (primeira faixa, segunda faixa e multiplicador) são exibidas tanto no display OLED quanto na matriz de LEDs.
- **Interface visual**: O display OLED exibe o valor numérico da resistência e as faixas de cores, enquanto a matriz de LEDs representa graficamente essas faixas.

## Componentes

- **RP2040 (BitDogLab)**
- **Resistor a ser medido**
- **Display OLED SSD1306**
- **Matriz de LEDs**
- **Botões para interação com o sistema**
- - **Jumpers e Protoboard**

## Funcionamento

1. **Leitura do resistor**: O código lê a resistência do resistor através do ADC do RP2040.
2. **Cálculo da resistência**: A resistência é calculada utilizando uma fórmula simples que leva em consideração a referência do resistor e o valor lido pelo ADC.
3. **Determinação das faixas de cor**: A série E24 é usada para identificar as faixas de cor correspondentes ao valor do resistor. As cores representam as faixas do resistor (primeira, segunda e multiplicador).
4. **Exibição no display e na matriz**: O valor da resistência e as cores das faixas são exibidos no display OLED. Além disso, a matriz de LEDs é configurada para exibir graficamente as cores correspondentes.

![Diagrama de Funcionamento](path/to/your/diagram.png)

## Como Funciona o Código

### 1. Inicialização do Sistema

O código começa com a inicialização de todos os periféricos necessários, incluindo a configuração do ADC, I2C para o display OLED, e GPIOs para os botões.

### 2. Leitura do Valor da Resistência

A função `adc_read()` é usada para realizar a leitura analógica da resistência conectada ao pino do RP2040. A leitura é então convertida para um valor de resistência utilizando uma fórmula específica.

### 3. Determinação das Faixas de Cor

A função `determinar_faixas()` determina os dois primeiros dígitos e o multiplicador do resistor. Esses valores são mapeados para cores, que são armazenadas e exibidas posteriormente.

### 4. Exibição no Display OLED e na Matriz de LEDs

O código utiliza a biblioteca **ssd1306** para enviar informações para o display OLED. O valor da resistência e as faixas de cores são exibidos de forma gráfica. Além disso, a matriz de LEDs exibe as cores correspondentes às faixas do resistor.

## Requisitos

- **Placa BitDogLab (RP2040)**
- **Display OLED SSD1306**
- **Matriz de LEDs (opcional)**
- **Resistores(da série E24)**
- **Ambiente de desenvolvimento com suporte a C e RP2040**

## Como Compilar e Executar

1. **Instale as dependências**: Certifique-se de que você tem o ambiente de desenvolvimento configurado para a placa RP2040. Você pode usar o **CMake** com o SDK do RP2040.
   
2. **Clone o repositório**:
   ```bash
   git clone https://github.com/Camila-RG/Ohmimetro-Resistores.git
   ```

3. **Compile o código** utilizando o seu ambiente de desenvolvimento preferido.

4. **Carregue o código na placa** e execute.

## Links

- Repositório GitHub: [https://github.com/Camila-RG/Ohmimetro-Resistores](https://github.com/Camila-RG/Ohmimetro-Resistores)
- [Link do vídeo Explicativo](https://drive.google.com/drive/folders/1H8pYX1uahgJAr60E5vb3V_4VGKynUMVj)

## Desenvolvedor

**Camila Ramos Gomes**

## Linguagem

**C**
