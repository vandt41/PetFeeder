/*
 * servo.h
 *
 *  Created on: May 3, 2026
 *      Author: brianmoser
 */

#ifndef BSP_SERVO_H_
#define BSP_SERVO_H_

#include "stm32f407xx.h"

void Servo_Init(void);
void Servo_SetAngle(uint8_t angle);

#endif /* BSP_SERVO_H_ */
