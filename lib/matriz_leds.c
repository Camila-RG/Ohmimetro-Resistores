#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "matriz_leds.h"

//imprimir valor binário
void imprimir_binario(int num) {
 int i;
 for (i = 31; i >= 0; i--) {
  (num & (1 << i)) ? printf("1") : printf("0");
 }
}

/*ws2812b_color_t colors[] = {
    {0, 0, 0},      // 0 - Preto
    {150, 0, 0},    // 1 - Marrom
    {255, 0, 0},    // 2 - Vermelho
    {255, 140, 0},  // 3 - Laranja
    {255, 255, 0},  // 4 - Amarelo
    {0, 255, 0},    // 5 - Verde
    {0, 255, 255},  // 6 - Azul
    {75, 0, 130},   // 7 - Violeta
    {128, 128, 128},// 8 - Cinza
    {255, 255, 255} // 9 - Branco
};*/


// Definição da intensidade dos leds
uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

//rotina para acionar a matrix de leds - ws2812
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        if (i%2==0)
        {
            valor_led = matrix_rgb(b = 0.0, desenho[24-i], desenho[24-i]);
            pio_sm_put_blocking(pio, sm, valor_led);

        }else{
            valor_led = matrix_rgb(b = 0.0, desenho[24-i], desenho[24-i]);
            pio_sm_put_blocking(pio, sm, valor_led);
        }
    }
}

void desenho_pio2(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        if (i%2==0)
        {
            valor_led = matrix_rgb(desenho[24-i], g = 0.0, r = 0.0);
            pio_sm_put_blocking(pio, sm, valor_led);

        }else{
            valor_led = matrix_rgb(desenho[24-i], g = 0.0, r = 0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
        }
    }
}

void desenho_pio3(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        if (i%2==0)
        {
            valor_led = matrix_rgb(r = 0.0, g = 0.0, desenho[24-i]);
            pio_sm_put_blocking(pio, sm, valor_led);

        }else{
            valor_led = matrix_rgb(r = 0.0, g = 0.0, desenho[24-i]);
            pio_sm_put_blocking(pio, sm, valor_led);
        }
    }
}