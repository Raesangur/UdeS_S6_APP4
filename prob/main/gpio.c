#include "globaldef.h"

uint16_t tx_pin = 23;
uint16_t rx_pin = 3;


void setup_gpio()
{
    //esp_rom_gpio_pad_select_gpio(tx_pin);
    gpio_set_direction(tx_pin, GPIO_MODE_OUTPUT);

    //esp_rom_gpio_pad_select_gpio(rx_pin);
    //gpio_set_direction(rx_pin, GPIO_MODE_INPUT);
}

void set_tx_gpio()
{
    printf("set\n");
    gpio_set_level(tx_pin, 1);
}

void clear_tx_gpio()
{
    printf("clear\n");
    gpio_set_level(tx_pin, 0);
}
