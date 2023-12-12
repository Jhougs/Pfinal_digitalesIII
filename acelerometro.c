#include "acelerometro.h"
#include "frecuencia.h"

const uint8_t ADDRESS = 0x1C;

extern myFlags_t banderas;
// Dirección del dispositivo I2C
const uint8_t REG_X_MSB = 0x01;
const uint8_t REG_X_LSB = 0x02;
const uint8_t REG_Y_MSB = 0x03;
const uint8_t REG_Y_LSB = 0x04;
const uint8_t REG_Z_MSB = 0x05;
const uint8_t REG_Z_LSB = 0x06;
const uint8_t REG_DATA_CFG = 0x0E;
const uint8_t REG_CTRL_REG1 = 0x2A;

// Configuraciones del acelerómetro
const uint8_t range_config = 0x01; 
const float count = 2048; 

uint8_t buf[2];

volatile bool timer_triggered = false;
extern volatile bool bandera_acelerom;

float x_acceleration;
float y_acceleration;
float z_acceleration;
volatile float mag_acceleration;

float convert_accel(uint16_t raw_accel) {
    float acceleration;
    if ((raw_accel & 0x2000) == 0x2000) {
        raw_accel &= 0x1FFF;
        acceleration = (-8192 + (float) raw_accel) / count;
    } else {
        acceleration = (float) raw_accel / count;
    }
    acceleration *= 9.81f;
    return acceleration;
}



void set_state(uint8_t state) {
    if(state == 0x01){
        buf[0] = REG_DATA_CFG;
        buf[1] = range_config;
        i2c_write_blocking(i2c_default, ADDRESS, buf, 2, false);
    }
    buf[0] = REG_CTRL_REG1;
    buf[1] = state; 
    i2c_write_blocking(i2c_default, ADDRESS, buf, 2, false);
    
}


void read_accelerometer(){
        i2c_write_blocking(i2c_default, ADDRESS, &REG_X_MSB, 1, true);
        i2c_read_blocking(i2c_default, ADDRESS, buf, 2, false);
        x_acceleration = convert_accel(buf[0] << 6 | buf[1] >> 2);

        i2c_write_blocking(i2c_default, ADDRESS, &REG_Y_MSB, 1, true);
        i2c_read_blocking(i2c_default, ADDRESS, buf, 2, false);
        y_acceleration = convert_accel(buf[0] << 6 | buf[1] >> 2);

        i2c_write_blocking(i2c_default, ADDRESS, &REG_Z_MSB, 1, true);
        i2c_read_blocking(i2c_default, ADDRESS, buf, 2, false);
        z_acceleration = convert_accel(buf[0] << 6 | buf[1] >> 2)-9.8;

        mag_acceleration = sqrt( pow(x_acceleration, 2)+pow(y_acceleration, 2)+pow(z_acceleration, 2));

        printf("Magnitud de la aceleración: %.6f \n ", mag_acceleration);
}



_Bool timer_callback(struct repeating_timer *t) {
    read_accelerometer(); // Llama a la función para leer los valores del acelerómetro cada segundo
    banderas.flags.acelerometro= true;
    return true;
}

