#ifndef MATRIZ_LEDS_H
#define MATRIZ_LEDS_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"  // Para clock_get_hz()
#include "hardware/pio.h"     // Para funções PIO

#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7

extern PIO pio;
extern uint sm;
extern uint led_r;
extern uint led_g;
extern uint led_b;

void imprimirbinario();
uint32_t matrix_rgb(double b, double r, double g);
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b);
void desenho_pio2(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b);
void desenho_pio3(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b);

#endif // MATRIZ_LEDS_H