/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>
#include <stdbool.h>
#include <stdio.h>

static bool bonMatin = false;

void bon_task(void* pvParameter)
{
    while(true)
    {
        if(bonMatin == true)
        {
            vTaskDelay(10);
        }
        else
        {
            printf("Bon ");
            bonMatin = true;
        }
    }
}

void matin_task(void* pvParameter)
{
    while(true)
    {
        if(bonMatin == false)
        {
            vTaskDelay(10);
        }
        else
        {
            printf("matin!\n");
            vTaskDelay(100);
            bonMatin = false;
        }
    }
}


void app_main()
{
    xTaskCreate(&bon_task, "bon_task", 2048, NULL, 5, NULL);
    xTaskCreate(&matin_task, "matin_task", 2048, NULL, 6, NULL);
}
