
#ifndef __MQTT_SENDING_H
#define __MQTT_SENDING_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "pico/time.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "pico/cyw43_arch.h"
#include "lwip/apps/mqtt_priv.h"
#include "tusb.h"
#include "wifi_mqtt.h"

typedef struct MQTT_CLIENT_DATA_T {
    mqtt_client_t *mqtt_client_inst;
    struct mqtt_connect_client_info_t mqtt_client_info;
    uint8_t data[MQTT_OUTPUT_RINGBUF_SIZE];
    uint8_t topic[100];
    uint32_t len;
    bool playing;
    bool newTopic;
} MQTT_CLIENT_DATA_T;

extern MQTT_CLIENT_DATA_T *mqtt;
extern struct mqtt_connect_client_info_t mqtt_client_info;

static void mqtt_pub_request_cb(void *arg, err_t result);
err_t publish_frecuencia(mqtt_client_t *client, void *arg,float frecuencia);
err_t publish_acelerometro(mqtt_client_t *client, void *arg,float frecuencia);
void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) ;
void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len);
void mqtt_request_cb(void *arg, err_t err) ;
void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);

#endif

