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

typedef struct {
    uint8_t r, g, b;
} Color;

Color coresRGB[] = {
    {0, 0, 0},         // Preto
    {150, 75, 0},      // Marrom
    {255, 0, 0},       // Vermelho
    {255, 140, 0},     // Laranja
    {255, 255, 0},     // Amarelo
    {0, 255, 0},       // Verde
    {0, 0, 255},       // Azul
    {148, 0, 211},     // Violeta
    {128, 128, 128},   // Cinza
    {255, 255, 255}    // Branco
};

void desenharResistor(int digito1, int digito2, int multiplicador) {
    Color c1 = coresRGB[digito1];
    Color c2 = coresRGB[digito2];
    Color c3 = coresRGB[multiplicador];

    uint32_t led1 = matrix_rgb(c1.b / 255.0, c1.r / 255.0, c1.g / 255.0);
    uint32_t led2 = matrix_rgb(c2.b / 255.0, c2.r / 255.0, c2.g / 255.0);
    uint32_t led3 = matrix_rgb(c3.b / 255.0, c3.r / 255.0, c3.g / 255.0);
    
    // Agora manda para a matriz:
    pio_sm_put_blocking(pio, sm, led1);
    pio_sm_put_blocking(pio, sm, led2);
    pio_sm_put_blocking(pio, sm, led3);
    
    // O resto dos LEDs você pode apagar ou deixar preto:
    for (int i = 3; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(pio, sm, 0);  // LED apagado (preto)
    }
}

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