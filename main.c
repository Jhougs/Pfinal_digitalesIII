 /**
 * @file main.c
 * @mainpage Archivo principal en el que se inicializan los puertos que se van a utilizar para la lectura de la información
 * ademas se inicializa la conexión WiFi y la conexión mqtt para dejarla establecida para el momento que sea necesario utilizarla
 * 
 * se usa en el archivo frecuencia.c y acelerometro.c. 
 * 
 * @author Juan José Viana Henao- Daniel Ovanny Mesa
 * 
 * @section Descripción 
* Además de incluir todas las librerías necesarias y los archivos de frecuencia,acelerometro, wifi y mqttt, se declaran variables tipo volatile ya que 
* se puede modificar en el algun punto del cldogi , indicando que se puede moficar en cualquier momento y se tiene que tener en cuenta este 
* valor modificado, luego se declara la variable que llevara el tiempo de enganche del sistema, sin embargo esta se documentará en 
* el archivo correspondiente donde se utilice , al igual que la variable chequeo_periodico y estado del sistema. En este punto, tambien se envía la información 
* para que sea publicada en el mqtt.
*/



#include "main.h"

/**
 * @param ADC_CHANNEL_2   Se define el canal ADC por el cual será leido la frecuencia cardiaca .
*/
#define ADC_CHANNEL ADC_CHANNEL_2

/**
 * @param ADC_PIN se define el GPIO donde se leerá la información de frecuencia cardiaca  .
*/
#define ADC_PIN 28
#define DEBUG_printf printf

/**
 * @param  WIFI_SSID se define la red WIFI a la cual se conectará el sistema 
*/
#define WIFI_SSID "FLIAVIANAH"
/**
 * @param  WIFI_PASSWORD se define la contraseña WIFI a la cual se conectará el sistema 
*/
#define WIFI_PASSWORD "JUANVIANA_"
/**
 * @param  IP se define la IP de la ubicación del broker MQTT 
*/
#define IP "31.220.107.127"

/**
 * @param  myFlags_t estructura que contiene las banderas para la escritura mqtt estas banderas contienen la permitividad
 * de escribir variables como acelerometro y frecuencia cardiaca
*/
extern volatile myFlags_t banderas;
/**
 * @param  frecuencia_cardiaca se captura la informacion de la frecuencia cardiaca leida  
*/
extern volatile float  frecuencia_cardiaca;
/**
 * @param  mag_acceleration se define la magnitud de la aceleracion que indica el nivel de actividad 
*/
extern volatile float mag_acceleration;


//#define MQTT_PORT 1883

/**
 * @brief funcion que inicializa los puertos necesarios para la lectura de los sensores  
*/
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
    //gpio_set_irq_enabled_with_callback(ADC_PIN, GPIO_IRQ_EDGE_RISE, true, &irq_handler_read_frec);

}


 
int main()
{
  init_ports();
  stdio_init_all();
  int sm = 0;

  mqtt=(MQTT_CLIENT_DATA_T*)calloc(1, sizeof(MQTT_CLIENT_DATA_T)); /** Se utiliza para reservar en memoria
  la información de conexión al Broker MQTT
    */

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

  cyw43_arch_enable_sta_mode();/** Funcion para realizar la conexión WiFi via sta mode que siginifica station mode
  este modo permite conectarse a una red wifi
    */
  if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) /** Se utiliza la función 
    cyw43_arch_wifi_connect_timeout_ms() para realizar la conexión wifi, aqui se ingresan el nombre de la red WIFI, la contraseña,
    el tipo de autentificación y el tiempo de conexion esperado
    */
  {
    printf("failed to connect\n");
    return 1;
  }

  ip_addr_t addr;
  if (!ip4addr_aton(IP, &addr)) {
    printf("ip error\n");
    return 0;
  }
  mqtt->mqtt_client_inst = mqtt_client_new(); /** Se crea el nuevo cliente MQTT para realizar la conexión */
  mqtt_set_inpub_callback(mqtt->mqtt_client_inst, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, mqtt);

  err_t err = mqtt_client_connect(mqtt->mqtt_client_inst, &addr, MQTT_PORT, &mqtt_connection_cb, mqtt, &mqtt->mqtt_client_info); /**  
    Se realiza la conexion con el cliente MQTT , en caso de no ser posible la conexión el sistema arroja que la conexión no ha sido posible  */
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
    /** Banderas que permiten escribir cuando la interrupción fue llamada. */
    if(banderas.flags.frecuencia){
      publish_frecuencia(mqtt ->mqtt_client_inst,mqtt,frecuencia_cardiaca);
      banderas.flags.frecuencia = false;
    }
    if(banderas.flags.acelerometro){
      publish_acelerometro(mqtt ->mqtt_client_inst,mqtt,mag_acceleration);
      banderas.flags.acelerometro= false;
    }
  }
   __wfi();
   

   
    return 0;
}