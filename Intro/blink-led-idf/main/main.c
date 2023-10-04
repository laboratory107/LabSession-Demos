#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define PIN_R GPIO_NUM_23
#define PIN_G GPIO_NUM_22
#define PIN_B GPIO_NUM_21


static void configure_led(void) {
    gpio_reset_pin(PIN_R);
    gpio_reset_pin(PIN_G);
    gpio_reset_pin(PIN_B);

    gpio_set_direction(PIN_R, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_G, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_B, GPIO_MODE_OUTPUT);
}


static uint8_t led_state = 0;
 
static void blink_led(void) {
    led_state = !led_state;
    gpio_set_level(R_PIN, led_state);
    gpio_set_level(G_PIN, !led_state);
}

void blink_task(void* param) {
    while (true) {
        blink_led();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


void app_main(void) {
    configure_led();
 
    xTaskCreate(&blink_task, "blink_task", 1024, NULL, 5, NULL);
}
