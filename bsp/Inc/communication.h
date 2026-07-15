/*
 * communication.h
 *
 *  Created on: Jun 18, 2026
 *      Author: brianmoser
 */

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdbool.h>
#include <string.h>
#include "stm32f407xx.h"
#include "gpio.h"
#include "usart.h"

#define COM_USART1					USART1
#define COM_USART1_GPIO_PORT		GPIOA
#define COM_USART1_GPIO_TX_PIN		GPIO_PIN_NO_9
#define COM_USART1_GPIO_RX_PIN		GPIO_PIN_NO_10

typedef uint8_t Command_t;
enum
{
    CMD_FEED = 0,
    CMD_FEED_COMPLETE,
    CMD_NTP_TIME_REQUEST,
    CMD_NTP_TIME_RESPONSE,
    CMD_STATUS_REQUEST,
    CMD_STATUS_RESPONSE,
    CMD_UPDATE_WIFI,
    CMD_ERROR
};

typedef struct __attribute__((packed))
{
	Command_t command;
    uint8_t value;
} CommandPacket_t;
typedef struct __attribute__((packed))
{
	Command_t command;
    uint32_t unixTime;
} TimePacket_t;
typedef struct __attribute__((packed))
{
    Command_t command;
    uint8_t bleStatus;
    uint8_t wifiStatus;
} StatusPacket_t;
void Communication_Init(void);
void Communication_Send(const void *buffer, uint16_t length);
bool Communication_Receive(void *buffer, uint16_t length);

#endif
