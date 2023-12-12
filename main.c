#include "main.h"

#define ADC_CHANNEL ADC_CHANNEL_2
#define ADC_RESOLUTION 12
#define ADC_SAMPLE_RATE 16000
#define ADC_PIN 28
#define DEBUG_printf printf
#define WIFI_SSID "FLIAVIANAH"
#define WIFI_PASSWORD "JUANVIANA_"
#define IP "31.220.107.127"

extern volatile myFlags_t banderas;
extern volatile float  frecuencia_cardiaca;
volatile bool bandera = false;
volatile bool bandera_acelerom = false;
extern volatile float mag_acceleration;


//#define MQTT_PORT 1883
void init_ports(){
    gpio_init(ADC_PIN);
    gpio_set_dir(ADC_PIN, GPIO_IN);
    adc_gpio_init(ADC_PIN);
    adc_init();
    adc_select_input(2);

    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    //gpio_set_irq_enabled_with_callback(ADC_PIN, GPIO_IRQ_EDGE_RISE, true, &irq_handler_read_frec2);

}


 /* resolve hostname to IP address: */
int main()
{
  init_ports();
  stdio_init_all();
  int sm = 0;

  mqtt=(MQTT_CLIENT_DATA_T*)calloc(1, sizeof(MQTT_CLIENT_DATA_T));

  if (!mqtt) {
      printf("mqtt client instant ini error\n");
      return 0;
  }
  mqtt->playing=false;
  mqtt->newTopic=false;
  mqtt->mqtt_client_info = mqtt_client_info;

  if (cyw43_arch_init())
  {
    printf("failed to initialise\n");
    return 1;
  }

  cyw43_arch_enable_sta_mode();
  if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000))
  {
    printf("failed to connect\n");
    return 1;
  }

  ip_addr_t addr;
  if (!ip4addr_aton(IP, &addr)) {
    printf("ip error\n");
    return 0;
  }
  mqtt->mqtt_client_inst = mqtt_client_new();
  mqtt_set_inpub_callback(mqtt->mqtt_client_inst, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, mqtt);

  err_t err = mqtt_client_connect(mqtt->mqtt_client_inst, &addr, MQTT_PORT, &mqtt_connection_cb, mqtt, &mqtt->mqtt_client_info);
  if (err != ERR_OK) {
    printf("connect error\n");
    return 0;
  }


  repeating_timer_t my_timer; 
  repeating_timer_callback_t my_callback = irq_handler_read_frec;
  add_repeating_timer_ms(1000, my_callback , NULL ,&my_timer);


  //gpio_set_irq_enabled_with_callback(ADC_PIN,GPIO_IRQ_EDGE_RISE,true,&irq_handler_read_frec2);

  // inicializacion del ACELEROMETROOOO 


    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    set_state(0x00);

    // Habilitar modo activo
    set_state(0x01);

    struct repeating_timer timer;

    // Configura un temporizador para llamar a timer_callback cada segundo
    add_repeating_timer_us(1000000, timer_callback, NULL, &timer);

    
    
  while(1) {

    if(banderas.flags.frecuencia){
      publish_frecuencia(mqtt ->mqtt_client_inst,mqtt,frecuencia_cardiaca);
      banderas.flags.frecuencia = false;
    }
    if(banderas.flags.acelerometro){
      publish_acelerometro(mqtt ->mqtt_client_inst,mqtt,mag_acceleration);
      banderas.flags.acelerometro= false;
    }
    //if(banderas.flags.frecuencia){
      sleep_ms(100);
      //banderas.flags.frecuencia = false;
    //}
        //example_publish(mqtt->mqtt_client_inst, mqtt);
        //sleep_ms(1000);
      /*
      if (mqtt->newTopic) { 
          mqtt->newTopic=false;
          //if (strcmp(mqtt->topic, "start")==0) {
          if (strcmp((const char*)mqtt->topic, "start") == 0) {
            printf("START");
          }
          //if (strcmp(mqtt->topic, "stop")==0) {
          if (strcmp((const char*)mqtt->topic, "stop") == 0) {
              printf("STOP");
          } 
      }
      */
   }
   __wfi();
   

   
    return 0;
}