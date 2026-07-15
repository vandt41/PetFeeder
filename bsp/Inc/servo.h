/*
 * servo.h
 *
 *  Created on: May 3, 2026
 *      Author: brianmoser
 */

#ifndef BSP_SERVO_H_
#define BSP_SERVO_H_

#include "stm32f407xx.h"
#include "rcc.h"
#include "gpio.h"

#define SERVO_PORT		GPIOA
#define SERVO_PIN		GPIO_PIN_NO_6

void Servo_Init(void);
void Servo_SetAngle(uint8_t angle);

#endif /* BSP_SERVO_H_ */
