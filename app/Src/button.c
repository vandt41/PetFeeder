/*
 * button.c
 *
 *  Created on: Jun 13, 2026
 *      Author: brianmoser
 */

#include "button.h"
void Button_Init(GPIO_RegDef_t* GPIO_Port, uint8_t GPIO_PinNumber)
{
	GPIO_Handle_t GPIOBtn = {0};

	GPIOBtn.pGPIOx = GPIO_Port;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PinNumber;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_PeriClockControl(GPIO_Port, ENABLE);
	GPIO_Init(&GPIOBtn);
}
//PD5 - interrupt on falling edge
void ExtBtn_Init(void)
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
uint8_t ExtBtn_IsPressed(void)
{
	return (GPIO_ReadFromInputPin(GPIOD, GPIO_PIN_NO_5));
}

uint8_t UserBtn_Init(void)
{
	GPIO_Handle_t GPIOBtn = {0};

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
	return 1;
}
uint8_t UserBtn_IsPressed(void)
{
	return (GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
}

static volatile uint8_t e_user_button_pressed = 0;

uint8_t Button_GetPressedEvent(void)
{
    uint8_t event = e_user_button_pressed;
    e_user_button_pressed = 0;

    return event;
}
void EXTI0_IRQHandler(void)
{
  // Handle the interrupt
  e_user_button_pressed = 1;
  GPIO_IRQHandling(GPIO_PIN_NO_0);
}
