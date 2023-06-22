/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "globaldef.h"
#include "gpio.h"
#include "message.h"
#include "serial_reception.h"
#include "timer.h"
#include "manchester.h"


void app_main()
{
    setup_timer(4800);
    setup_gpio();

    xTaskCreate(&reception_task, "Serial Reception", 2048, NULL, 5, NULL);
    
    char* data;
    size_t dataLength;
    char* receptionData;
    char* end;

    
    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(1));
        if(has_serial_reception())
        {
            create_message(get_serial_buffer());
            data = get_tx_data_buffer();
            printf("Sending: %s\n", data);
            dataLength = strlen(data);
            receptionData = get_rx_raw_data_buffer();
            end = receptionData + dataLength;
        }
        if (get_and_clear_reception_byte() >= end)
        {
            printf("Sent: %s\nRecv: %s\n", data, receptionData);
            memset(receptionData, '\0', dataLength);
            clear_raw_data_buffer();
        }
        

    }
}
