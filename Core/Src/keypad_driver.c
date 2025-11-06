#include "keypad_driver.h"
#include "main.h"

static const char keypad_map[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void keypad_init(keypad_handle_t* keypad) {
    // Inicializar todas las filas en estado BAJO para permitir detección
    for (int i = 0; i < KEYPAD_ROWS; i++) {
        HAL_GPIO_WritePin(keypad->row_ports[i], keypad->row_pins[i], GPIO_PIN_RESET);
    }
}

char keypad_scan(keypad_handle_t* keypad, uint16_t col_pin) {
    char key_pressed = '\0';
    int col_index = -1;

    // Identificar la columna que generó la interrupción
    for (int i = 0; i < KEYPAD_COLS; i++) {
        if (col_pin == keypad->col_pins[i]) {
            col_index = i;
            break;
        }
    }

    if (col_index == -1) {
        return '\0';
    }

    // Debounce delay
    HAL_Delay(10);

    // Escanear filas para encontrar la tecla presionada
    for (int row = 0; row < KEYPAD_ROWS; row++) {
        // Poner todas las filas en ALTO
        for (int i = 0; i < KEYPAD_ROWS; i++) {
            HAL_GPIO_WritePin(keypad->row_ports[i], keypad->row_pins[i], GPIO_PIN_SET);
        }

        // Poner solo la fila actual en BAJO
        HAL_GPIO_WritePin(keypad->row_ports[row], keypad->row_pins[row], GPIO_PIN_RESET);

        // Pequeña pausa para estabilización
        HAL_Delay(1);

        // Verificar si la columna está en BAJO (tecla presionada)
        if (HAL_GPIO_ReadPin(keypad->col_ports[col_index], keypad->col_pins[col_index]) == GPIO_PIN_RESET) {
            key_pressed = keypad_map[row][col_index];
            
            // Esperar hasta que se suelte la tecla
            while (HAL_GPIO_ReadPin(keypad->col_ports[col_index], keypad->col_pins[col_index]) == GPIO_PIN_RESET) {
                HAL_Delay(10);
            }
            break;
        }
    }

    // Restaurar configuración inicial
    keypad_init(keypad);
    
    return key_pressed;
}