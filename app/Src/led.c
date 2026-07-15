/*
 * led.c
 *
 *  Created on: Jun 13, 2026
 *      Author: brianmoser
 */
#include <led.h>
static void LED_InitPin(uint8_t pin)
{
    GPIO_Handle_t led;
	memset(&led, 0, sizeof(led));

    led.pGPIOx = LED_PORT;
    led.GPIO_PinConfig.GPIO_PinNumber = pin;
    led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    led.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    GPIO_Init(&led);
}

void Led_Init(void)
{
    GPIO_PeriClockControl(LED_PORT, ENABLE);

    LED_InitPin(LED_HEARTBEAT_PIN);
    LED_InitPin(LED_FEED_PIN);
    LED_InitPin(LED_ERROR_PIN);
    LED_InitPin(LED_UART_PIN);

    GPIO_WriteToOutputPin(LED_PORT, LED_HEARTBEAT_PIN, OFF);
    GPIO_WriteToOutputPin(LED_PORT, LED_FEED_PIN, OFF);
    GPIO_WriteToOutputPin(LED_PORT, LED_ERROR_PIN, OFF);
    GPIO_WriteToOutputPin(LED_PORT, LED_UART_PIN, OFF);
}

void LED_HeartbeatToggle(void)
{
    GPIO_ToggleOutputPin(LED_PORT, LED_HEARTBEAT_PIN);
}

void LED_CommPulse(void)
{
    GPIO_ToggleOutputPin(LED_PORT, LED_UART_PIN);
}
void LED_FeedOn(void){
	GPIO_WriteToOutputPin(LED_PORT, LED_FEED_PIN, ON);
}
void LED_FeedOff(void)
{
	GPIO_WriteToOutputPin(LED_PORT, LED_FEED_PIN, OFF);

}
void LED_ErrorOn(void)
{
	GPIO_WriteToOutputPin(LED_PORT, LED_ERROR_PIN, ON);

}
void LED_ErrorOff(void)
{
	GPIO_WriteToOutputPin(LED_PORT, LED_ERROR_PIN, OFF);

}
