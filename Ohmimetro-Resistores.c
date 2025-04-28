#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "lib/ssd1306.h"
#include "lib/font.h"
#include "lib/matriz_leds.h"

// Definições globais
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define ADC_PIN 28
#define Botao_A 5

int R_conhecido = 10000;
float ADC_VREF = 3.31;
int ADC_RESOLUTION = 4095;

// Cores
const char *cores[] = {
    "Preto", "Marrom", "Vermelho", "Laranja", "Amarelo",
    "Verde", "Azul", "Violeta", "Cinza", "Branco"
};

// Determinar faixas
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

int main() {
    // Inicialização
    gpio_init(Botao_A);
    gpio_set_dir(Botao_A, GPIO_IN);
    gpio_pull_up(Botao_A);

    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_t ssd;
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);

    adc_init();
    adc_gpio_init(ADC_PIN);

    const char *cor1 = " ";
    const char *cor2 = " ";
    const char *cor3 = " ";

    char str_x[10];
    char str_y[10];

    bool cor = true;
    while (true) {
        adc_select_input(2);

        float soma = 0.0f;
        for (int i = 0; i < 500; i++) {
            soma += adc_read();
            sleep_ms(1);
        }
        float media = soma / 500.0f;

        float R_x = (R_conhecido * media) / (ADC_RESOLUTION - media);

        // Atualiza str_x e str_y
        snprintf(str_x, sizeof(str_x), "%d", (int)media);
        snprintf(str_y, sizeof(str_y), "%.0f", R_x);

        int digito1, digito2, multiplicador;
        determinar_faixas(R_x, &digito1, &digito2, &multiplicador);

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

        // Atualiza o display
        ssd1306_fill(&ssd, !cor);
        ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);
        ssd1306_draw_string(&ssd, "Resist. Cores:", 10, 4);
        ssd1306_line(&ssd, 3, 12, 123, 12, cor);
        ssd1306_line(&ssd, 44, 37, 44, 60, cor);
        ssd1306_draw_string(&ssd, "1.", 8, 16);
        ssd1306_draw_string(&ssd, "2.", 8, 24);
        ssd1306_draw_string(&ssd, "3.", 8, 32);
        ssd1306_draw_string(&ssd, cor1, 20, 16);
        ssd1306_draw_string(&ssd, cor2, 20, 24);
        ssd1306_draw_string(&ssd, cor3, 20, 32);
        ssd1306_draw_string(&ssd, "ADC", 13, 45);
        ssd1306_draw_string(&ssd, "Resisten.", 50, 45);
        ssd1306_draw_string(&ssd, str_x, 8, 52);
        ssd1306_draw_string(&ssd, str_y, 59, 52);
        ssd1306_send_data(&ssd);

        sleep_ms(700);
    }
}
