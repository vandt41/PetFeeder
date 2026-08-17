/*
 * communication.c
 *
 *  Created on: Jun 18, 2026
 *      Author: brianmoser
 */
#include <communication.h>
#include <stdio.h>
#include "feeder.h"

/*
 * Note: We are using USART2 here
 * GPIOA ALT FUNCTION 7
 * TX: PA2
 * RX: PA3
 * */
USART_Handle_t usart2_handle;

static CommandPacket_t rxPacket;
static volatile bool packetReady = false;
// TX : PA2
// RX : PA3
uint8_t Communication_Init(void)
{
    USART_PeripheralControl(USART2, DISABLE);
    GPIO_PinConfig_t gpio_usart2_tx_conf;
    gpio_usart2_tx_conf.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    gpio_usart2_tx_conf.GPIO_PinAltFunMode = 7;
    gpio_usart2_tx_conf.GPIO_PinMode = GPIO_MODE_ALTFN;
    gpio_usart2_tx_conf.GPIO_PinNumber = GPIO_PIN_NO_2;
    gpio_usart2_tx_conf.GPIO_PinPuPdControl = GPIO_PIN_PU;
    gpio_usart2_tx_conf.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GPIO_Handle_t gpio_usart2_tx_handle;
    gpio_usart2_tx_handle.pGPIOx = GPIOA;
    gpio_usart2_tx_handle.GPIO_PinConfig = gpio_usart2_tx_conf;

    GPIO_PinConfig_t gpio_usart2_rx_conf;
    gpio_usart2_rx_conf.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    gpio_usart2_rx_conf.GPIO_PinAltFunMode = 7;
    gpio_usart2_rx_conf.GPIO_PinMode = GPIO_MODE_ALTFN;
    gpio_usart2_rx_conf.GPIO_PinNumber = GPIO_PIN_NO_3;
    gpio_usart2_rx_conf.GPIO_PinPuPdControl = GPIO_PIN_PU;
    gpio_usart2_rx_conf.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GPIO_Handle_t gpio_usart2_rx_handle;
    gpio_usart2_rx_handle.pGPIOx = GPIOA;
    gpio_usart2_rx_handle.GPIO_PinConfig = gpio_usart2_rx_conf;

    USART_Config_t usart2_conf;
    usart2_conf.USART_Baud = USART_STD_BAUD_115200;
    usart2_conf.USART_Mode = USART_MODE_TXRX;
    usart2_conf.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
    usart2_conf.USART_NoOfStopBits = USART_STOPBITS_1;
    usart2_conf.USART_ParityControl = USART_PARITY_DISABLE;
    usart2_conf.USART_WordLength = USART_WORDLEN_8BITS;

//    USART_Handle_t usart2_handle;
    usart2_handle.pUSARTx = USART2;
    usart2_handle.USART_Config = usart2_conf;
    USART_Init(&usart2_handle);
    USART_PeripheralControl(USART2, ENABLE);

    GPIO_Init(&gpio_usart2_tx_handle);
    GPIO_Init(&gpio_usart2_rx_handle);
    return 1;
}

void Communication_Send(const void *buffer, uint16_t length)
{
	USART_SendData(&usart2_handle, (uint8_t *)buffer, length);
}

bool Communication_Receive(void *buffer, uint16_t length)
{
    USART_ReceiveData(&usart2_handle, (uint8_t *)buffer, length);

    return true;
}

void Communication_IRQHandler(void)
{
    USART_ReceiveData(&usart2_handle, (uint8_t *)&rxPacket, sizeof(rxPacket));

    packetReady = true;
}
void Communication_Process(void)
{
    if(!packetReady)
        return;

    packetReady = false;

    switch(rxPacket.command)
    {
        case CMD_FEED:

            Feeder_FeedOnce();

            CommandPacket_t reply =
            {
                .command = CMD_FEED_COMPLETE,
                .value = rxPacket.value
            };

            Communication_Send(&reply,sizeof(reply));

            break;

        case CMD_NTP_TIME_RESPONSE:

            // update RTC

            break;

        default:
            break;
    }
}
