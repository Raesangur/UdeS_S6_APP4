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
    setup_timer(100);
    setup_gpio();

    // xTaskCreate(&reception_task, "Serial Reception", 2048, NULL, 5, NULL);

    create_message("Bon matin!");
    char* data = get_tx_data_buffer();
    printf("%s\n", data);

    char receptionData[700] = {'\0'};
    int i = 0;

    while(true)
    {
        //vTaskDelay(pdMS_TO_TICKS(1));
        //if(has_serial_reception())
        //{
        //    printf("%s", get_serial_buffer());
        //}
        char receivedByte = get_and_clear_reception_byte();
        if (receivedByte != '\0')
        {
            receptionData[i++] = receivedByte;
            printf("%s\n", receptionData);
            if (receive_message(data))
            {
                char* receivedString = get_rx_data_buffer();
                if (receivedString)
                {
                    printf("Received: %s\n", receivedString);
                    memset(receptionData, '\0', sizeof(receptionData));
                }
            }
        }
        
    }
}
