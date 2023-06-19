#include "globaldef.h"

uint16_t tx_pin = 1;
uint16_t rx_pin = 3;


void setup_gpio()
{
    gpio_pad_select_gpio(tx_pin);
    gpio_set_direction(tx_pin, GPIO_MODE_OUTPUT);

    gpio_pad_select_gpio(INPUT_PIN);
    gpio_set_direction(INPUT_PIN, GPIO_MODE_INPUT);
}

void set_tx_gpio()
{
    gpio_set_level(tx_pin, 1);
}

void clear_tx_gpio()
{
    gpio_set_level(tx_pin, 0);
}
