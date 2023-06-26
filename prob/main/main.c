/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "globaldef.h"
#include "gpio.h"
#include "manchester.h"
#include "message.h"
#include "serial_reception.h"
#include "timer.h"
#include "transmission.h"


void app_main()
{
    setup_timer(5000);
    setup_gpio();

    xTaskCreate(&reception_task, "Serial Reception", 2048, NULL, 5, NULL);
    xTaskCreate(&transmission_task, "Transmission Task", 2048, NULL, 5, NULL);

    char* receptionData = get_rx_raw_data_buffer();


    // Uncomment this line to add an error after CRC computation when sending the data
    add_error_to_message();

    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(1));

        if(esp_timer_get_time() - get_last_reception_time() > 100 * get_clock_us()
           && get_current_data_pointer() != NULL && get_current_data_pointer() != (NULL - 1))
        {
            receptionData[0] = '0';
            if(!receive_message(receptionData))
            {
                printf("Recv: %s\n", receptionData);
                char* rx_data_buffer = get_rx_data_buffer();
                if(rx_data_buffer)
                    printf(
                      "%s\nTotal Reception Time: %lld ticks\nTotal Decoding Time: %lld ticks\n",
                      rx_data_buffer,
                      get_and_clear_reception_time(),
                      get_and_clear_decoding_time());
                memset(receptionData, '\0', strlen(receptionData));
                ready_raw_data_buffer();
            }
        }
    }
}
