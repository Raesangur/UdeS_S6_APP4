#include "globaldef.h"
#include "manchester.h"


static void timer_callback(void* param)
{
    printf("Bon matin\n");
    transmitBit();
}


void setup_timer(uint32_t baudrate)
{
    const uint32_t timer_us = (1.0 / baudrate) * 1000000 * 2;

    const esp_timer_create_args_t timer_args = {
              .callback = &timer_callback,
              .name = "Manchester timer"};
              
    esp_timer_handle_t timer_handler;
    ESP_ERROR_CHECK(esp_timer_create(&timer_args, &timer_handler));
    ESP_ERROR_CHECK(esp_timer_start_periodic(timer_handler, timer_us));
}
