/*
 * feeder.c
 *
 *  Created on: May 4, 2026
 *      Author: brianmoser
 */
#include "feeder.h"

volatile uint8_t feeding_triggered = 0;

void Feeder_FeedOnce(void)
{
	Servo_SetAngle(90); // Open position
	delay_ms(500);// Keep open for 500ms
	Servo_SetAngle(0);   // Close position
}

void Feeder_Run(void)
{
	if (feeding_triggered)
	{
		Feeder_FeedOnce();
		feeding_triggered = 0;
	}
}

//GPIOA6
void Feeder_Init(void)
{
	Servo_Init();
}
/*
void EXTI0_IRQHandler(void)
{
	// Handle the interrupt
	GPIO_IRQHandling(BUTTON_GPIO_USERBTN_PIN);
	feeding_triggered = 1;
}
*/

