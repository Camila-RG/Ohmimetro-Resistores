#ifndef MATRIZ_LEDS_H
#define MATRIZ_LEDS_H

#include "pio_matrix.pio.h"

#define WS2812_PIN 7

extern PIO pio;
extern uint sm;
extern uint led_r;
extern uint led_g;
extern uint led_b;

// Definição de tipo da estrutura que irá controlar a cor dos LED's
typedef struct {
    double red;
    double green;
    double blue;
}Led_config;

typedef struct {
    int r;
    int g;
    int b;
} led;

typedef Led_config RGB_cod;

// Definição de tipo da matriz de leds
typedef Led_config Matriz_leds_config[5][5]; 

uint32_t gerar_binario_cor(double red, double green, double blue);

uint configurar_matriz(PIO pio, uint8_t pin);

void imprimir_desenho(Matriz_leds_config configuracao, PIO pio, uint sm);

RGB_cod obter_cor_por_parametro_RGB(int red, int green, int blue);

void hex_to_rgb(uint32_t hex_animation[][25], led data[][25], uint8_t frames);

void ajustar_brilho(led matriz[][25], float brightness, uint8_t FRAME_COUNT);

void converter_para_RGB_matriz_leds(led matriz[5][5], Matriz_leds_config matriz_leds);

void enviar_animacao(led matriz[][25], PIO pio, uint sm, uint8_t FRAME_COUNT);

void limpar_matriz(PIO pio, uint sm);

#endif // MATRIZ_LEDS_H