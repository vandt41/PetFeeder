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
extern USART_Handle_t usart2_handle;

#define STATUS_FEEDER      (1U << 0)
#define STATUS_UART        (1U << 1)
#define STATUS_LED         (1U << 2)
#define STATUS_BUTTON      (1U << 3)
#define STATUS_RTC         (1U << 4)
#define STATUS_ERROR       (1U << 5)

typedef uint8_t Command_t;
enum
{
	CMD_ERROR = 0,
    CMD_FEED,
    CMD_FEED_COMPLETE,
	CMD_STATUS_REQUEST,
	CMD_STATUS_RESPOND,
    CMD_NTP_TIME_REQUEST,
    CMD_NTP_TIME_RESPONSE,
    CMD_INIT,
	CMD_DEINIT,
    CMD_INIT_COMPLETE
};

typedef enum
{
	ERR_ERROR_NONE = 0,
	ERR_FEEDER_JAM,
	ERR_FEEDER_OVERTEMP,
	ERR_FEEDER_EMPTY,
    ERR_INVALID_COMMAND,
    ERR_COMMUNICATION,
	ERR_INVALID_PORTION
}ErrorState_t;

typedef struct __attribute__((packed))
{
	Command_t command;
    uint8_t value;
} CommandPacket_t;

uint8_t Communication_Init(void);
void Communication_Send(const void *buffer, uint16_t length);
//bool Communication_Receive(void *buffer, uint16_t length);
bool Communication_Receive(CommandPacket_t *packet);

void Communication_IRQHandler(void);
void Communication_Process(void);

uint8_t USART_ReadByte(USART_Handle_t *pUSARTHandle);
bool USART_IsRxDataAvailable(USART_Handle_t *pUSARTHandle);
#endif
