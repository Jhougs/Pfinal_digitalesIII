
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

/**
 * @brief Estructura que contiene los datos del cliente MQTT.
 * 
 * Esta estructura almacena la información relacionada con el cliente MQTT,
 * incluyendo la instancia del cliente, la información de conexión, buffers para datos y temas,
 * así como indicadores para control de reproducción y temas nuevos.
 */
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

/**
 * @brief Estructura que contiene la información de conexión del cliente MQTT.
 * 
 * Esta estructura almacena los detalles necesarios para establecer la conexión del cliente MQTT,
 * incluyendo el ID del cliente, el nombre de usuario, la contraseña, el tiempo de vida de la sesión, entre otros.
 */
extern struct mqtt_connect_client_info_t mqtt_client_info;

/**
 * @brief Callback para manejar el resultado de una solicitud de publicación MQTT.
 * 
 * @param arg Argumento adicional.
 * @param result Estado del resultado de la solicitud (ERR_OK si es exitoso, otro código de error en caso contrario).
 */
static void mqtt_pub_request_cb(void *arg, err_t result);

/**
 * @brief Publica la frecuencia cardíaca en el servidor MQTT.
 * 
 * @param client Cliente MQTT para publicar los datos.
 * @param arg Argumento adicional.
 * @param frecuencia Valor de la frecuencia cardíaca a publicar.
 * @return Estado de la publicación.
 */
err_t publish_frecuencia(mqtt_client_t *client, void *arg,float frecuencia);

/**
 * @brief Publica la aceleración en el servidor MQTT.
 * 
 * @param client Cliente MQTT para publicar los datos.
 * @param arg Argumento adicional.
 * @param aceleracion Valor de la aceleración a publicar.
 * @return Estado de la publicación.
 */
err_t publish_acelerometro(mqtt_client_t *client, void *arg,float frecuencia);

/**
 * @brief Callback llamado cuando se recibe un mensaje MQTT.
 * 
 * @param arg Argumento adicional.
 * @param data Datos recibidos.
 * @param len Longitud de los datos.
 * @param flags Marcadores de los datos.
 */
void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags);

/**
 * @brief Callback llamado cuando se recibe un mensaje MQTT.
 * 
 * @param arg Argumento adicional.
 * @param topic Tema del mensaje recibido.
 * @param tot_len Longitud total del mensaje recibido.
 */
void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len);

/**
 * @brief Callback llamado para informar sobre el estado de una solicitud MQTT.
 * 
 * @param arg Argumento adicional.
 * @param err Estado de la solicitud MQTT.
 */
void mqtt_request_cb(void *arg, err_t err) ;

/**
 * @brief Callback llamado al establecerse o cambiar el estado de la conexión MQTT.
 * 
 * @param client Cliente MQTT relacionado con la conexión.
 * @param arg Argumento adicional.
 * @param status Estado de la conexión MQTT (e.g., MQTT_CONNECT_ACCEPTED, MQTT_CONNECT_DISCONNECTED, etc.).
 */
void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);

#endif

