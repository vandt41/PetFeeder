/*
 * system.c
 *
 *  Created on: Sep 6, 2026
 *      Author: brianmoser
 */
#include <stdio.h>
#include "led.h"
#include "feeder.h"
#include "communication.h"
#include "system.h"

static volatile ErrorState_t g_error = ERR_ERROR_NONE;

void Error_Handle(ErrorState_t error)
{
	g_error = error;
    switch (error)
    {
        case ERR_INVALID_PORTION:
            // Reject feeding request

            break;

        case ERR_FEEDER_JAM:
			break;
        case ERR_TIMEOUT_WIFI:
            // Record/report error


        case ERR_TIMEOUT_BLE:
            // Record/report error

			g_error = ERR_ERROR_NONE;
			printf("Turning off BLE\n");
			LED_Blue_Off();
			break;

        case ERR_TIMEOUT_RTC:
            // Retry RTC / evaluate RTC state
            break;

        case ERR_WIFI_DISCONNECT:
            // Continue operating locally
            break;

        case ERR_OTA_FAIL:
            // Continue current firmware
            break;

        case ERR_RTC_FAIL:
            // Disable scheduled feeding

        	break;

        default:
            break;
    }
}

uint8_t System_HasError(void)
{
	uint8_t event = g_error;
	return event;
}
