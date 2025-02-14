#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

#define MIC_ADC_PIN 26  // Pino ADC onde o microfone está conectado
#define LED_MATRIX_PIN_BASE 2  // Pino base da matriz de LEDs
#define LED_MATRIX_SIZE 8  // Número de LEDs na matriz

void init_adc() {
    adc_init();
    adc_gpio_init(MIC_ADC_PIN);
    adc_select_input(0);  // Seleciona a entrada ADC 0 (GPIO26)
}

void init_led_matrix() {
    for (int i = 0; i < LED_MATRIX_SIZE; i++) {
        gpio_init(LED_MATRIX_PIN_BASE + i);
        gpio_set_dir(LED_MATRIX_PIN_BASE + i, GPIO_OUT);
    }
}

void set_led_matrix(uint16_t sound_level) {
    int leds_to_light = (sound_level * LED_MATRIX_SIZE) / 4096;  // Normaliza para 8 LEDs
    for (int i = 0; i < LED_MATRIX_SIZE; i++) {
        gpio_put(LED_MATRIX_PIN_BASE + i, i < leds_to_light);
    }
}

uint16_t read_sound_level() {
    return adc_read(); // Lê o valor do ADC (0-4095 para 12 bits)
}

int main() {
    stdio_init_all();
    init_adc();
    init_led_matrix();
    printf("Medidor de Som Iniciado!\n");

    while (1) {
        uint16_t sound_level = read_sound_level();
        printf("Nível de Som: %d\n", sound_level);
        set_led_matrix(sound_level);
        sleep_ms(500);  // Aguarda meio segundo entre leituras
    }
}
