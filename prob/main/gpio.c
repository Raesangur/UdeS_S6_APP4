#include "globaldef.h"
#include "manchester.h"


uint16_t tx_pin = 23;
uint16_t rx_pin = 22;

static void IRAM_ATTR gpio_interrupt_handler(void* args)
{
    receiveBit();
}


void setup_gpio()
{
    // esp_rom_gpio_pad_select_gpio(tx_pin);
    gpio_set_direction(tx_pin, GPIO_MODE_OUTPUT);

    // esp_rom_gpio_pad_select_gpio(rx_pin);
    gpio_set_direction(rx_pin, GPIO_MODE_INPUT);
    gpio_set_intr_type(rx_pin, GPIO_INTR_ANYEDGE);
    gpio_pulldown_en(rx_pin);
    gpio_pullup_dis(rx_pin);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(rx_pin, gpio_interrupt_handler, NULL);
}

void set_tx_gpio()
{
    gpio_set_level(tx_pin, 1);
}

void clear_tx_gpio()
{
    gpio_set_level(tx_pin, 0);
}

bool read_rx_gpio()
{
    return gpio_get_level(rx_pin);
}
