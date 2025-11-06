#include "led_driver.h"

void led_init(led_handle_t *led) {
    // Asegurar que el LED empiece apagado
    HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_RESET);
}

void led_on(led_handle_t *led) {
    HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_SET);
}

void led_off(led_handle_t *led) {
    HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_RESET);
}

void led_toggle(led_handle_t *led) {
    HAL_GPIO_TogglePin(led->port, led->pin);
}

void led_blink(led_handle_t *led, uint32_t duration_ms, uint32_t blink_count) {
    for (uint32_t i = 0; i < blink_count; i++) {
        led_toggle(led);
        HAL_Delay(duration_ms);
        led_toggle(led);
        if (i < blink_count - 1) HAL_Delay(duration_ms);
    }
}