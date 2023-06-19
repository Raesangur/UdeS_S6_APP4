/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <stdbool.h>
#include <esp_system.h>
#include <esp_timer.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

const static uint32_t timer_us = 1 * 1000000;
static bool bonMatinFlag = false;

void timer_callback(void* param)
{
    bonMatinFlag = true;
}

void app_main()
{
    const esp_timer_create_args_t timer_args = {
          .callback = &timer_callback,
          .name = "Bon matin timer"};
    esp_timer_handle_t timer_handler;
    ESP_ERROR_CHECK(esp_timer_create(&timer_args, &timer_handler));
    ESP_ERROR_CHECK(esp_timer_start_periodic(timer_handler, timer_us));

    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(100));

        if (bonMatinFlag)
        {
            bonMatinFlag = false;
            printf("Bon matin\n");
        }
    }
}
