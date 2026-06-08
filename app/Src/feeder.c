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
	delay_ms(10);// Keep open for 500ms
	Servo_SetAngle(0);   // Close position
}

void Feeder_ButtonInit(void)
{
	GPIO_Handle_t ButtonPin;
	ButtonPin.pGPIOx = GPIOA;
	ButtonPin.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	ButtonPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT; // Rising edge trigger
	ButtonPin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	ButtonPin.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	ButtonPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU; // External pull-down on Discovery board

	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_Init(&ButtonPin);
	// IRQ configuration
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI0, NVIC_IRQ_PRI15);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI0, ENABLE);
}

void Feeder_Run(void)
{
	if (feeding_triggered)
	{
		Feeder_FeedOnce();
		feeding_triggered = 0;
	}
}

void EXTI0_IRQHandler(void)
{
	// Handle the interrupt
	GPIO_IRQHandling(GPIO_PIN_NO_0);
	feeding_triggered = 1;
}
