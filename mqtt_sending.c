#include "mqtt_sending.h"




MQTT_CLIENT_DATA_T *mqtt;

struct mqtt_connect_client_info_t mqtt_client_info =
{
  "prueba",
  "sensores", /* user */
  "Juanviana10_", /* pass */
  60,  /* keep alive */
  NULL, /* will_topic */
  NULL, /* will_msg */
  2,    /* will_qos */
  0     /* will_retain */
#if LWIP_ALTCP && LWIP_ALTCP_TLS
  , NULL
#endif
};

/* Called when publish is complete either with sucess or failure */
static void mqtt_pub_request_cb(void *arg, err_t result)
{
  if(result != ERR_OK) {
    printf("Publish result: %d\n", result);
  }
}


err_t publish_frecuencia(mqtt_client_t *client, void *arg,float frecuencia)
{
  
  char frec_to_transmit[10];
  sprintf(frec_to_transmit,"%f",frecuencia);
  printf("el valor int es %s\n", frec_to_transmit);
  err_t err;
  u8_t qos = 2; /* 0 1 or 2, see MQTT specification */
  u8_t retain = 0; /* No don't retain such crappy payload... */
  cyw43_arch_lwip_begin();
  err = mqtt_publish(client, "frecuencia", frec_to_transmit, strlen(frec_to_transmit), qos, retain, mqtt_pub_request_cb, arg);
  cyw43_arch_lwip_end();
  if(err != ERR_OK) {
    printf("Publish err: %d\n", err);
  }
  return err;
}

err_t publish_acelerometro(mqtt_client_t *client, void *arg,float aceleracion)
{
  
  char ace_to_transmit[10];
  sprintf(ace_to_transmit,"%f",aceleracion);
  err_t err;
  u8_t qos = 2; /* 0 1 or 2, see MQTT specification */
  u8_t retain = 0; /* No don't retain such crappy payload... */
  cyw43_arch_lwip_begin();
  err = mqtt_publish(client, "sensores/acelerometro", ace_to_transmit, strlen(ace_to_transmit), qos, retain, mqtt_pub_request_cb, arg);
  cyw43_arch_lwip_end();
  if(err != ERR_OK) {
    printf("Publish err: %d\n", err);
  }
  return err;
}


void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
    printf("mqtt_incoming_data_cb");
    MQTT_CLIENT_DATA_T* mqtt_client = (MQTT_CLIENT_DATA_T*)arg;
    LWIP_UNUSED_ARG(data);

    //strncpy(mqtt_client->data, data, len);
    strncpy((char*)mqtt_client->data, (char*)data, len);

    mqtt_client->len=len;
    mqtt_client->data[len]='\0';
    
    mqtt_client->newTopic=true;
    mqtt->playing=false;
 
}

void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len) {
  printf("mqtt_incoming_publish_cb");
  MQTT_CLIENT_DATA_T* mqtt_client = (MQTT_CLIENT_DATA_T*)arg;
  //strcpy(mqtt_client->topic, topic);
  strcpy((char*)mqtt_client->topic, (char*)topic);
}

void mqtt_request_cb(void *arg, err_t err) {
  MQTT_CLIENT_DATA_T* mqtt_client = ( MQTT_CLIENT_DATA_T*)arg;

  LWIP_PLATFORM_DIAG(("MQTT client \"%s\" request cb: err %d\n", mqtt_client->mqtt_client_info.client_id, (int)err));
}

void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
  MQTT_CLIENT_DATA_T* mqtt_client = (MQTT_CLIENT_DATA_T*)arg;
  LWIP_UNUSED_ARG(client);

  LWIP_PLATFORM_DIAG(("MQTT client \"%s\" connection cb: status %d\n", mqtt_client->mqtt_client_info.client_id, (int)status));

  if (status == MQTT_CONNECT_ACCEPTED) {
    printf("MQTT_CONNECT_ACCEPTED");


    //example_publish(client, arg);
    //mqtt_disconnect(client);

    
    mqtt_sub_unsub(client,
            "start", 0,
            mqtt_request_cb, arg,
            1);
    mqtt_sub_unsub(client,
            "stop", 0,
            mqtt_request_cb, arg,
            1);
    
    
  }
}

