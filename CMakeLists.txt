cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(final_project) # nombre de nuestro proyecto
set(PICO_BOARD pico_w)
pico_sdk_init()



add_executable(final_project
        main.c
        frecuencia.c
        acelerometro.c
        mqtt_sending.c
        
        # keyboard.c
        )




target_link_libraries(final_project pico_cyw43_arch_lwip_threadsafe_background
pico_lwip_mbedtls
pico_lwip_mqtt
pico_stdlib
hardware_adc
hardware_gpio
hardware_i2c)

target_include_directories(final_project PRIVATE C:/Users/jjviana/Desktop/projectos/final_project/paho.mqtt.c-master/src)




pico_enable_stdio_usb(final_project 1) 
pico_enable_stdio_uart(final_project 0)

pico_add_extra_outputs(final_project)

