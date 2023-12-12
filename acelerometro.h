#ifndef __ACELEROMETRO_H
#define __ACELEROMETRO_H

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "wifi_mqtt.h"
#include "math.h"

// Dirección del dispositivo I2C



float convert_accel(uint16_t raw_accel);
/**
 * @brief Configura el estado del acelerómetro.
 * 
 * @param state Estado a configurar en el acelerómetro.
 */
void set_state(uint8_t state);

/**
 * @brief Realiza la lectura y muestra los valores del acelerómetro.
 */
void read_accelerometer();

/**
 * @brief Función de devolución de llamada para el temporizador.
 * 
 * @param t Puntero al temporizador.
 */
_Bool timer_callback(struct repeating_timer *t);

#endif