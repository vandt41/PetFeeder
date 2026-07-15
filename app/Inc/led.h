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

#define LED_UART_PIN 			GPIO_PIN_NO_12
#define LED_FEED_PIN         	GPIO_PIN_NO_13
#define LED_ERROR_PIN        	GPIO_PIN_NO_14
#define LED_HEARTBEAT_PIN    	GPIO_PIN_NO_15

void Led_Init();
void LED_HeartbeatToggle(void);
void LED_FeedOn(void);
void LED_FeedOff(void);
void LED_ErrorOn(void);
void LED_ErrorOff(void);
void LED_CommPulse(void);
#endif /* LED_H_ */
