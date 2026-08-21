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
	LED_Green_On();
	Servo_SetAngle(180); // Open position
	delay_ms(500);// Keep open for 500ms
	Servo_SetAngle(0);   // Close position
	LED_Green_Off();
}

/*
 *	Current time/portion: 500ms
 */
void Feeder_Portion(uint8_t portion)
{
	Servo_SetAngle(90); // Open position
	delay_ms(500 * portion);
	Servo_SetAngle(0);   // Close position
}
uint8_t Feeder_HandleJam(void)
{
	uint8_t res;
	for(uint8_t i = 0; i < 5; i++)
	{
		if(Servo_GetAngle() < 10)
			{
				res = 0;
				break;
			}
		else
		{
			Servo_SetAngle(10);
			delay_ms(200);
			Servo_SetAngle(0);
		}
		res = 1;
	}
	return res;
}

//GPIOA6
uint8_t Feeder_Init(void)
{
	Servo_Init();
	return 1;
}
/*
void EXTI0_IRQHandler(void)
{
	// Handle the interrupt
	GPIO_IRQHandling(BUTTON_GPIO_USERBTN_PIN);
	feeding_triggered = 1;
}
*/

