/**
 * @file frecuencia.h
 * @brief Definiciones y funciones para el cálculo de frecuencia cardíaca.
 */

#ifndef __FRECUENCIA_H
#define __FRECUENCIA_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "pico/time.h"


#define ADC_CHANNEL ADC_CHANNEL_2
#define ADC_RESOLUTION 12
#define ADC_SAMPLE_RATE 16000

/**
 * @brief Estructura de banderas para control de múltiples flags.
 * 
 * Esta estructura define un conjunto de banderas para controlar múltiples estados mediante bits individuales.
 * Utiliza una unión entre un entero de 8 bits y una estructura de bits anidada para definir varios flags.
 */
typedef union {
  uint8_t banderas;
  struct 
  {
    bool frecuencia :1;
    bool temperatura:1;
    bool acelerometro:1;
    uint8_t         :5;
  } flags;
} myFlags_t;


/**
 * @brief Manejador de interrupción para leer la frecuencia cardíaca.
 * 
 * Este manejador se ejecuta en respuesta a una interrupción de temporizador repetitivo.
 * Llama a la función `read_adc_value()` para leer y calcular la frecuencia cardíaca,
 * y establece la bandera correspondiente.
 * 
 * @param rt Puntero al temporizador repetitivo.
 * @return Valor booleano indicando si se ha manejado la interrupción correctamente.
 */

bool irq_handler_read_frec(repeating_timer_t *rt);

/**
 * @brief Lee el valor del ADC y calcula la frecuencia cardíaca.
 * 
 * Lee el valor del ADC, convierte el valor leído a voltaje, y calcula la frecuencia cardíaca.
 * El cálculo se realiza mediante una fórmula basada en el valor leído del ADC.
 * Si el valor leído es mayor que 1.17, se calcula la frecuencia cardíaca.
 * En caso contrario, se establece la frecuencia cardíaca como 0.
 */
void read_adc_value();



#endif