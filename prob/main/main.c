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
    
    char* data = NULL;
    size_t dataLength = 0;
    char* receptionData = NULL;
    char* end = NULL;

    
    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(1));
        if(has_serial_reception())
        {
            char* serialData = get_serial_buffer();
            create_message(serialData);
            data = get_tx_data_buffer();
            printf("Sending: %s - %s\n", serialData, data);
            
            dataLength = strlen(data);
            receptionData = get_rx_raw_data_buffer();
            end = receptionData + dataLength;

            ready_transmission();

            set_reception_flag();
        }
        if (end != NULL && receptionData != NULL && data != NULL)
        {
            if (get_and_clear_reception_byte() >= end)
            {
                clear_raw_data_buffer();
                receptionData[0] = '0';
                printf("Sent: %s\nRecv: %s\n", data, receptionData);
                receive_message(receptionData);
                printf("%s\n", get_rx_data_buffer());
                memset(receptionData, '\0', dataLength);
                ready_raw_data_buffer();
            }
        }
    }
}
