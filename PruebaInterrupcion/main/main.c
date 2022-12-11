#include <stdio.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include<esp_log.h>

#define boton 27
int c =0;
const char *tag = "prueba interrupcion";


void saluda(void){
    printf("has pulsado el boton");
}

void funcionHandler(void *args){ //no puede tener ni printf ni log
 c=1;
}

//inicializa interrupcion
esp_err_t init_interrupcion(void){

    gpio_config_t configuracion;
    // 1ULL es un sinonimo de unsigned long long 
    //devemos transformar nuestro 27(16 bits) en un long long
    //ya que pin bits mask es un unit64_t(64 bits)
    configuracion.pin_bit_mask = (1ULL << boton);  
    configuracion.mode = GPIO_MODE_DEF_INPUT;
    configuracion.pull_up_en = GPIO_PULLUP_DISABLE;
    configuracion.pull_down_en = GPIO_PULLUP_DISABLE;  //tenemos una externa
    configuracion.intr_type = GPIO_INTR_NEGEDGE; //FLANCO DE BAJADA

    gpio_config(&configuracion);

    //habilita interrupciones globalmente
    gpio_install_isr_service(0);

    //agregamos nuestro pin a las interrupciones
    gpio_isr_handler_add(boton,funcionHandler,NULL);

    return ESP_OK;
}

void app_main() {
    init_interrupcion();
    int x =0;
    while(1){
        printf("\n %d",x);
        //ESP_LOGI(tag,"\n %d",x);
        vTaskDelay(100);
        if(c==1){
            printf("\nhas pulsado el boton");
            x=0;
            c =0;
        }
        x++;
    }
}