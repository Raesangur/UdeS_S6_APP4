/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "globaldef.h"
#include "gpio.h"
#include "serial_reception.h"
#include "timer.h"


void app_main()
{
    setup_timer(1);
    setup_gpio();

    xTaskCreate(&reception_task, "Serial Reception", 2048, NULL, 5, NULL);

    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
        if(has_serial_reception())
        {
            printf("%s", get_serial_buffer());
        }
    }
}
