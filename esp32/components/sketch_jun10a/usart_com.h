#ifndef USART_COM_H
#define USART_COM_H

#include "stdint.h"
#include <Arduino.h>
// Define TX and RX pins for USART2
#define TXD2 17
#define RXD2 16
extern HardwareSerial MySerial;

enum Command_t : uint8_t
{
    CMD_ERROR = 0,
    CMD_SUCCESS,
    CMD_FEED,
    CMD_FEED_COMPLETE,
    CMD_NTP_TIME_REQUEST,
    CMD_NTP_TIME_RESPONSE,
    CMD_INIT
};

typedef struct __attribute__((packed))
{
    Command_t command;
    uint8_t value;
} CommandPacket_t;

void USART_Init();
// void sendTimePacket(Command_t cmd, uint32_t unixTime);
void sendCmdPacket(Command_t cmd, uint8_t value);
bool readCmdPacket(CommandPacket_t &pkt);
#endif
