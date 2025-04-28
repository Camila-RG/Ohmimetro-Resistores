#include <stdio.h>
 #include <stdlib.h>
 #include "pico/stdlib.h"
 #include "hardware/adc.h"
 #include "hardware/i2c.h"
 #include "lib/ssd1306.h"
 #include "lib/font.h"

 // Definição de variáveis globais
 #define I2C_PORT i2c1
 #define I2C_SDA 14
 #define I2C_SCL 15
 #define endereco 0x3C
 #define ADC_PIN 28 // GPIO para o voltímetro
 #define Botao_A 5  // GPIO para botão A
 
 int R_conhecido = 10000;   // Resistor de 10k ohm
 float R_x = 0.0;           // Resistor desconhecido
 float ADC_VREF = 3.31;     // Tensão de referência do ADC
 int ADC_RESOLUTION = 4095; // Resolução do ADC (12 bits)
 
// Mapeamento de cores
const char *cores[] = {
    "Preto",    // 0
    "Marrom",   // 1
    "Vermelho", // 2
    "Laranja",  // 3
    "Amarelo",  // 4
    "Verde",    // 5
    "Azul",     // 6
    "Violeta",  // 7
    "Cinza",    // 8
    "Branco"    // 9
};

// Função para mapear o valor do ADC para a faixa de cor
const char* mapear_cor_resistor(uint16_t valor_adc) {
    // Mapeamento de valores de ADC para faixas de cor
    if (valor_adc < 410) return cores[1];  // Marrom
    if (valor_adc < 820) return cores[2];  // Vermelho
    if (valor_adc < 1230) return cores[3]; // Laranja
    if (valor_adc < 1640) return cores[4]; // Amarelo
    if (valor_adc < 2050) return cores[5]; // Verde
    if (valor_adc < 2460) return cores[6]; // Azul
    if (valor_adc < 2870) return cores[7]; // Violeta
    if (valor_adc < 3280) return cores[8]; // Cinza
    return cores[9]; // Branco
}

 // Trecho para modo BOOTSEL com botão B
 #include "pico/bootrom.h"
 #define botaoB 6
 void gpio_irq_handler(uint gpio, uint32_t events)
 {
   reset_usb_boot(0, 0);
 }
 
 const float tabela_E24[] = {
    510, 560, 620, 680, 750, 820, 910,
    1000, 1100, 1200, 1300, 1500, 1600, 1800, 2000, 2200,
    2400, 2700, 3000, 3300, 3600, 3900, 4300, 4700, 5100,
    5600, 6200, 6800, 7500, 8200, 9100,
    10000, 11000, 12000, 13000, 15000, 16000, 18000, 20000,
    22000, 24000, 27000, 30000, 33000, 36000, 39000, 43000,
    47000, 51000, 56000, 62000, 68000, 75000, 82000, 91000, 100000
  };

  const int num_valores = sizeof(tabela_E24) / sizeof(tabela_E24[0]);

  void determinar_faixas(float resistencia, int *digito1, int *digito2, int *multiplicador) {
    int valor = (int)resistencia;
    *multiplicador = 0;
    while (valor >= 100) {
        valor /= 10;
        (*multiplicador)++;
    }
    *digito1 = valor / 10;
    *digito2 = valor % 10;
}

 int main()
 {
   // Para ser utilizado o modo BOOTSEL com botão B
   gpio_init(botaoB);
   gpio_set_dir(botaoB, GPIO_IN);
   gpio_pull_up(botaoB);
   gpio_set_irq_enabled_with_callback(botaoB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
   // Aqui termina o trecho para modo BOOTSEL com botão B
 
   gpio_init(Botao_A);
   gpio_set_dir(Botao_A, GPIO_IN);
   gpio_pull_up(Botao_A);
 
   // I2C Initialisation. Using it at 400Khz.
   i2c_init(I2C_PORT, 400 * 1000);
 
   gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
   gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
   gpio_pull_up(I2C_SDA);                                        // Pull up the data line
   gpio_pull_up(I2C_SCL);                                        // Pull up the clock line
   ssd1306_t ssd;                                                // Inicializa a estrutura do display
   ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
   ssd1306_config(&ssd);                                         // Configura o display
   ssd1306_send_data(&ssd);                                      // Envia os dados para o display
 
   // Limpa o display. O display inicia com todos os pixels apagados.
   ssd1306_fill(&ssd, false);
   ssd1306_send_data(&ssd);
 
   adc_init();
   adc_gpio_init(ADC_PIN); // GPIO 28 como entrada analógica
 
   uint16_t valor_adc = adc_read(); // Lê o valor do ADC (0-4095)
   const char *corR = mapear_cor_resistor(valor_adc);

   const char *cor1 = " "; 
   const char *cor2 = " "; 
   const char *cor3 = " ";

   float tensao;
   char str_x[5]; // Buffer para armazenar a string
   char str_y[5]; // Buffer para armazenar a string
 
   bool cor = true;
   while (true)
   {
     adc_select_input(2); // Seleciona o ADC para eixo X. O pino 28 como entrada analógica
 
     float soma = 0.0f;
     for (int i = 0; i < 500; i++)
     {
       soma += adc_read();
       sleep_ms(1);
     }
     float media = soma / 500.0f;
 
    // Fórmula simplificada: R_x = R_conhecido * ADC_encontrado /(ADC_RESOLUTION - adc_encontrado)
    R_x = (R_conhecido * media) / (ADC_RESOLUTION - media);

    int digito1, digito2, multiplicador;
    determinar_faixas(R_x, &digito1, &digito2, &multiplicador);

    // Agora fazer if-else para buscar cores
    if (digito1 >= 0 && digito1 <= 9)
        cor1 = cores[digito1];
    else
        cor1 = "Erro";

    if (digito2 >= 0 && digito2 <= 9)
        cor2 = cores[digito2];
    else
        cor2 = "Erro";

    if (multiplicador >= 0 && multiplicador <= 9)
        cor3 = cores[multiplicador];
    else
        cor3 = "Erro";
 
    // cor = !cor;
    // Atualiza o conteúdo do display com animações
    ssd1306_fill(&ssd, !cor);                          // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);       // Desenha um retângulo
    ssd1306_draw_string(&ssd, "Resist. Cores:", 10, 4);  // Desenha título
    ssd1306_line(&ssd, 3, 12, 123, 12, cor);             // Linha horizontal
    ssd1306_line(&ssd, 44, 37, 44, 60, cor);             // Linha vertical separando ADC e Resistência
    ssd1306_draw_string(&ssd, "1.", 8, 16);              // Numeros "1.", "2.", "3." alinhados
    ssd1306_draw_string(&ssd, "2.", 8, 24);
    ssd1306_draw_string(&ssd, "3.", 8, 32);
    ssd1306_draw_string(&ssd, cor1, 8, 16);             // Exibe o nome da cor da faixa 1
    ssd1306_draw_string(&ssd, cor2, 8, 24);              // Exibe o nome da cor da faixa 2
    ssd1306_draw_string(&ssd, cor3, 8, 32);              // Exibe o nome da cor da faixa 3
    ssd1306_draw_string(&ssd, "ADC", 13, 45);            // Escreve "ADC" no lado esquerdo
    ssd1306_draw_string(&ssd, "Resisten.", 50, 45);      // Escreve "Resisten." no lado direito
    ssd1306_draw_string(&ssd, str_x, 8, 52);             // Mostra valor ADC
    ssd1306_draw_string(&ssd, str_y, 59, 52);            // Mostra valor Resistência
    ssd1306_send_data(&ssd);                             // Atualiza o display
    sleep_ms(700);
   }
 }