#ifndef __FRECUENCIA_H
#define __FRECUENCIA_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "pico/time.h"


#define ADC_CHANNEL ADC_CHANNEL_2
#define ADC_RESOLUTION 12
#define ADC_SAMPLE_RATE 16000


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



bool irq_handler_read_frec(repeating_timer_t *rt);
void read_adc_value();



#endif