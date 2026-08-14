/*
 * led.h
 *
 *  Created on: Jun 13, 2026
 *      Author: brianmoser
 */

#ifndef LED_H_
#define LED_H_

#include "stm32f407xx.h"
#include "gpio.h"
#include <string.h>

#define ON			1
#define OFF			0

#define LED_PORT 	GPIOD

#define GREEN_LED_PIN 			GPIO_PIN_NO_12
#define ORANGE_LED_PIN         	GPIO_PIN_NO_13
#define RED_LED_PIN        		GPIO_PIN_NO_14
#define BLUE_LED_PIN    		GPIO_PIN_NO_15

void Led_Init();
void LED_Blue_On(void);
void LED_Blue_Off(void);
void LED_Red_On(void);
void LED_Red_Off(void);
void LED_Green_On(void);
void LED_Green_Off(void);
void LED_CommPulse(void);
#endif /* LED_H_ */
