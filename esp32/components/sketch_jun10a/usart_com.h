#ifndef USART_COM_H
#define USART_COM_H

#include "stdint.h"
#include <Arduino.h>
// Define TX and RX pins for USART1
#define TXD1 19
#define RXD1 21
typedef enum
{
    CMD_FEED = 0,
    CMD_FEED_COMPLETE,
    CMD_NTP_TIME_REQUEST,
    CMD_NTP_TIME_RESPONSE,
    CMD_STATUS_REQUEST,
    CMD_STATUS_RESPONSE,
    CMD_UPDATE_WIFI,
    CMD_ERROR
} Command_t;

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
// Use Serial1 for UART communication
void USART_Init();
void sendStatusPacket(uint8_t bleStatus, uint8_t wifiStatus);
void sendTimePacket(Command_t cmd, uint32_t unixTime);
void sendCmdPacket(Command_t cmd, uint8_t value);
bool readCmdPacket(CommandPacket_t &pkt);
#endif
