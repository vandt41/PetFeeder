/*
 * button.c
 *
 *  Created on: Jun 13, 2026
 *      Author: brianmoser
 */

#include "button.h"
void Button_Init(GPIO_RegDef_t* GPIO_Port, uint8_t GPIO_PinNumber)
{
	GPIO_Handle_t GPIOBtn;

	GPIOBtn.pGPIOx = GPIO_Port;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PinNumber;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_PeriClockControl(GPIO_Port, ENABLE);
	GPIO_Init(&GPIOBtn);
}
void Button_It_Init(void)
{
	GPIO_Handle_t GPIOBtn;

	GPIOBtn.pGPIOx = GPIOD;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	GPIOBtn.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_PeriClockControl(GPIOD, ENABLE);
	GPIO_Init(&GPIOBtn);

	// IRQ configuration
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI9_5, NVIC_IRQ_PRI15);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5, ENABLE);
}
void UserBtn_It_Init(void)
{
	GPIO_Handle_t GPIOBtn;
	memset(&GPIOBtn, 0, sizeof(GPIOBtn));
	GPIOBtn.pGPIOx = BUTTON_GPIO_USERBTN_PORT;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = BUTTON_GPIO_USERBTN_PIN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_RT;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIOBtn.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_PeriClockControl(BUTTON_GPIO_USERBTN_PORT, ENABLE);
	GPIO_Init(&GPIOBtn);

	// IRQ configuration
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI0, NVIC_IRQ_PRI15);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI0, ENABLE);
}

