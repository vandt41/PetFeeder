/*
 * communication.c
 *
 *  Created on: Jun 18, 2026
 *      Author: brianmoser
 */
#include <communication.h>
static uint8_t rx_buffer[2];
static volatile uint8_t packet_received = 0;

USART_Handle_t usart2_handle;

/*
 * Note: We are using USART2 here
 * GPIOA ALT FUNCTION 7
 * TX: PA2
 * RX: PA3
 * */
uint8_t Communication_Init(void)
{
    USART_PeripheralControl(USART2, DISABLE);

    /* ================= GPIO TX ================= */

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


    /* ================= GPIO RX ================= */

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


    /* ================= USART ================= */

    USART_Config_t usart2_conf;

    usart2_conf.USART_Baud = USART_STD_BAUD_115200;
    usart2_conf.USART_Mode = USART_MODE_TXRX;
    usart2_conf.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
    usart2_conf.USART_NoOfStopBits = USART_STOPBITS_1;
    usart2_conf.USART_ParityControl = USART_PARITY_DISABLE;
    usart2_conf.USART_WordLength = USART_WORDLEN_8BITS;

    usart2_handle.pUSARTx = USART2;
    usart2_handle.USART_Config = usart2_conf;

    /* Initialize USART */
    USART_Init(&usart2_handle);

    /* Initialize GPIO */
    GPIO_Init(&gpio_usart2_tx_handle);
    GPIO_Init(&gpio_usart2_rx_handle);

    /* Enable USART2 interrupt in NVIC */
    USART_IRQInterruptConfig(IRQ_NO_USART2, ENABLE);

    /* Optional: set priority */
    USART_IRQPriorityConfig(IRQ_NO_USART2, NVIC_IRQ_PRI5);

    /* Enable USART peripheral */
    USART_PeripheralControl(USART2, ENABLE);
    USART_ReceiveDataIT(&usart2_handle, rx_buffer, 2);
    return 1;
}


void Communication_Send(const void *buffer, uint16_t length)
{
	USART_SendData(&usart2_handle, (uint8_t *)buffer, length);
}

bool Communication_Receive(CommandPacket_t *packet)
{
    if (!packet_received)
    {
        return false;
    }

    packet_received = 0;

    packet->command = (Command_t)rx_buffer[0];
    packet->value   = rx_buffer[1];

    // Re-arm reception for the next packet
    USART_ReceiveDataIT(&usart2_handle, rx_buffer, 2);

    return true;
}


void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle, uint8_t event)
{
    if (event == USART_EVENT_RX_CMPLT)
    {
        packet_received = 1;
    }
}


void USART2_IRQHandler(void)
{
	USART_IRQHandling(&usart2_handle);
}
