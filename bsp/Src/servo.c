/*
 * servo.c
 *
 *  Created on: May 3, 2026
 *      Author: brianmoser
 */

#include "servo.h"

void Servo_Init(void)
{
	// 1. Enable TIM3 clock
	TIM3_PCLK_EN();

	// 2. Configure PA6 as TIM3_CH1 (AF2)
	GPIO_Handle_t ServoPin;
	ServoPin.pGPIOx = SERVO_PORT;
	ServoPin.GPIO_PinConfig.GPIO_PinNumber = SERVO_PIN;
	ServoPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	ServoPin.GPIO_PinConfig.GPIO_PinAltFunMode = 2;
	ServoPin.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	ServoPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	ServoPin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_PeriClockControl(SERVO_PORT, ENABLE);
	GPIO_Init(&ServoPin);

	// 3. Configure TIM3 for PWM
	// Calculate Timer clock frequency
	uint32_t pclk1 = RCC_GetPCLK1Value();
	uint32_t tim_clk;
	if (((RCC->CFGR >> 10) & 0x7) < 4) {
		tim_clk = pclk1;
	} else {
		tim_clk = 2 * pclk1;
	}

	// Set Prescaler to get 1MHz (1us per tick)
	TIM3->PSC = (tim_clk / 1000000) - 1;

	// Set Auto-reload for 50Hz (20ms period)
	TIM3->ARR = 20000 - 1;

	// Configure Channel 1 as PWM mode 1 (OC1M = 110)
	TIM3->CCMR1 &= ~(0x7 << 4); // Clear bits
	TIM3->CCMR1 |= (0x6 << 4);  // PWM mode 1
	TIM3->CCMR1 |= (1 << 3);    // OC1PE: Output compare 1 preload enable

	// Enable Channel 1 output
	TIM3->CCER |= (1 << 0);

	// Set initial angle (0 degrees -> 1000us)
	TIM3->CCR1 = 1000;

	// Enable the counter
	TIM3->CR1 |= (1 << 0);
}

void Servo_SetAngle(uint8_t angle)
{
	if (angle > 180) angle = 180;

	// Map 0-180 degrees to 1000-2000us
	// CCR = 1000 + (angle * 1000 / 180)
	uint32_t pulse_width = 1000 + (angle * 1000 / 180);
	TIM3->CCR1 = pulse_width;
}
