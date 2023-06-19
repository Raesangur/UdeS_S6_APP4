/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "globaldef.h"
#include "timer.h"

void app_main()
{
    setup_timer(1);
    
    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
