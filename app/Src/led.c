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
/* Onboard LED Init
 * PD12
 * PD13
 * PD14
 * PD15
 * */
void Led_Init(void)
{
    GPIO_PeriClockControl(LED_PORT, ENABLE);

    LED_InitPin(GREEN_LED_PIN);
    LED_InitPin(ORANGE_LED_PIN);
    LED_InitPin(RED_LED_PIN);
    LED_InitPin(BLUE_LED_PIN);

    GPIO_WriteToOutputPin(LED_PORT, GREEN_LED_PIN, OFF);
    GPIO_WriteToOutputPin(LED_PORT, ORANGE_LED_PIN, OFF);
    GPIO_WriteToOutputPin(LED_PORT, RED_LED_PIN, OFF);
    GPIO_WriteToOutputPin(LED_PORT, BLUE_LED_PIN, OFF);
}

void LED_Blue_On(void)
{
	GPIO_WriteToOutputPin(LED_PORT, BLUE_LED_PIN, ON);
}
void LED_Blue_Off(void)
{
	GPIO_WriteToOutputPin(LED_PORT, BLUE_LED_PIN, OFF);
}
void LED_Green_On(void){
	GPIO_WriteToOutputPin(LED_PORT, GREEN_LED_PIN, ON);
}
void LED_Green_Off(void)
{
	GPIO_WriteToOutputPin(LED_PORT, GREEN_LED_PIN, OFF);

}
void LED_Red_On(void)
{
	GPIO_WriteToOutputPin(LED_PORT, RED_LED_PIN, ON);

}
void LED_Red_Off(void)
{
	GPIO_WriteToOutputPin(LED_PORT, RED_LED_PIN, OFF);

}
