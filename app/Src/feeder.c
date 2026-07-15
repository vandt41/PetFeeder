/*
 * feeder.c
 *
 *  Created on: May 4, 2026
 *      Author: brianmoser
 */
#include "feeder.h"

volatile uint8_t feeding_triggered = 0;
void delay_ms(uint32_t ms)
{
	// Roughly 1ms at 16MHz HSI if each iteration is ~4 cycles
	for (uint32_t i = 0; i < (ms * 4000); i++)
	{
		__asm("nop");
	}
}
void Feeder_FeedOnce(void)
{
	Servo_SetAngle(90); // Open position
	delay_ms(50);// Keep open for 500ms
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
/*
void EXTI0_IRQHandler(void)
{
	// Handle the interrupt
	GPIO_IRQHandling(BUTTON_GPIO_USERBTN_PIN);
	feeding_triggered = 1;
}
*/

