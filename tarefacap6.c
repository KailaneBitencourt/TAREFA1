#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"
#include "matrizes.h"

// Definições de GPIO
#define LED_PIN_BLUE 12
#define LED_PIN_GREEN 11
#define LED_PIN_RED 13
#define BUTTON_PIN_A 5
#define BUTTON_PIN_B 6
#define LED_MATRIX_PIN 7
#define I2C_PORT i2c1
#define SDA_PIN 14
#define SCL_PIN 15
#define SSD1306_ADDR 0x3C

PIO pio = pio0;
uint sm = 0;

void init_funcionalidades() {
    stdio_init_all();

    // LEDs RGB
    gpio_init(LED_PIN_BLUE);
    gpio_set_dir(LED_PIN_BLUE, GPIO_OUT);
    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);

    // Botões
    gpio_init(BUTTON_PIN_A);
    gpio_set_dir(BUTTON_PIN_A, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_A);
    gpio_init(BUTTON_PIN_B);
    gpio_set_dir(BUTTON_PIN_B, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_B);

    // Inicializa WS2812
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, LED_MATRIX_PIN, 800000, false);

    // Inicializa o display SSD1306
    i2c_init(I2C_PORT, 100000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
}

void verifica_botoes() {
    static bool led_state_a = false; 
    static bool led_state_b = false; 

    if (gpio_get(BUTTON_PIN_A) == 0) {
        led_state_a = !led_state_a; 
        gpio_put(LED_PIN_GREEN, led_state_a);
        printf("Botão A pressionado!\n");
        sleep_ms(200);  // Debounce
    }

    if (gpio_get(BUTTON_PIN_B) == 0) {
        led_state_b = !led_state_b; 
        gpio_put(LED_PIN_BLUE, led_state_b);
        printf("Botão B pressionado!\n");
        sleep_ms(200);  // Debounce
    }
}


int main() {
    init_funcionalidades();

    while (true) {
        verifica_botoes(); 
        sleep_ms(10); 

    }

    return 0;
}
