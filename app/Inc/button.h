/*
 * servo.h
 *
 *  Created on: May 3, 2026
 *      Author: brianmoser
 */

#ifndef BSP_BUTTON_H_
#define BSP_BUTTON_H_

#include "stm32f407xx.h"
#include "gpio.h"
#include <string.h>

#define BUTTON_GPIO_USERBTN_PORT	GPIOA
#define BUTTON_GPIO_USERBTN_PIN		GPIO_PIN_NO_0

void UserBtn_It_Init(void);
void Button_It_Init(void);
void Button_Init(GPIO_RegDef_t* GPIO_Port, uint8_t GPIO_PinNumber);

#endif /* BSP_BUTTON_H_ */
