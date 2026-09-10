/*
 * system.h
 *
 *  Created on: Sep 6, 2026
 *      Author: brianmoser
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_


typedef enum
{
	ERR_ERROR_NONE = 0,
    ERR_FEEDER_JAM,
    ERR_FEEDER_OVERTEMP,
    ERR_FEEDER_EMPTY,
	ERR_INVALID_PORTION,
    ERR_TIMEOUT_WIFI,
    ERR_TIMEOUT_BLE,
    ERR_TIMEOUT_RTC,
    ERR_WIFI_DISCONNECT,
    ERR_OTA_FAIL,
    ERR_RTC_FAIL
} ErrorState_t;

void Error_Handle(ErrorState_t error);
uint8_t System_HasError(void);

#endif /* SYSTEM_H_ */
