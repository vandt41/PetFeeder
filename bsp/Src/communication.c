/*
 * communication.c
 *
 *  Created on: Jun 18, 2026
 *      Author: brianmoser
 */
#include <communication.h>

/*
 * Note: We are using USART1 here
 * GPIOA ALT FUNCTION 7
 * TX: PA9
 * RX: PA10
 * */
static USART_Handle_t s_usart1Handle;
void Communication_Init(void)
{
    GPIO_Handle_t usartGPIO;

    usartGPIO.pGPIOx = COM_USART1_GPIO_PORT;
    usartGPIO.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    usartGPIO.GPIO_PinConfig.GPIO_PinAltFunMode = 7;
    usartGPIO.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    usartGPIO.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    usartGPIO.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    /* PA2 -> USART2_TX */
    usartGPIO.GPIO_PinConfig.GPIO_PinNumber = COM_USART1_GPIO_TX_PIN;
    GPIO_Init(&usartGPIO);
    /* PA3 -> USART2_RX */
    usartGPIO.GPIO_PinConfig.GPIO_PinNumber = COM_USART1_GPIO_RX_PIN;
    GPIO_Init(&usartGPIO);

    s_usart1Handle.pUSARTx = COM_USART1;
    s_usart1Handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;
    s_usart1Handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
    s_usart1Handle.USART_Config.USART_Mode = USART_MODE_TXRX;
    s_usart1Handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
    s_usart1Handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
    s_usart1Handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;

    USART_Init(&s_usart1Handle);
    USART_PeripheralControl(COM_USART1, ENABLE);
}

void Communication_Send(const void *buffer, uint16_t length)
{
	USART_SendData(
	        &s_usart1Handle,
	        (uint8_t *)buffer,
	        length);
}
bool Communication_Receive(void *buffer, uint16_t length)
{
    USART_ReceiveData(
    		&s_usart1Handle,
	        (uint8_t *)buffer,
	        length);
    return true;
}


