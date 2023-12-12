#include "frecuencia.h"


myFlags_t banderas;
volatile float frecuencia_cardiaca;
void read_adc_value(){

    int lectura_adc = adc_read();

    // Calcular la frecuencia cardíaca
    float lectura  = (float)lectura_adc / 4095.0 * 3.3;
    lectura = lectura - 0.5;
    //frecuencia_cardiaca = (lectura*150)/ (3.3);
    if(lectura >1.13){
        frecuencia_cardiaca = (lectura*150)/ (3.3);
    } else {
        frecuencia_cardiaca = 0;
    }
    
    printf("Frecuencia cardíaca: %.2f BPM\n", frecuencia_cardiaca);
}

bool irq_handler_read_frec(repeating_timer_t *rt){
    read_adc_value();
    banderas.flags.frecuencia = true;

}



