#ifndef FEEDER_H_
#define FEEDER_H_

#include "servo.h"
#include "button.h"
uint8_t Feeder_Init(void);//GPIOA6
void Feeder_FeedOnce(void);
void Feeder_Portion(uint8_t portion);
uint8_t Feeder_HandleJam(void);
extern void delay_ms(uint32_t ms);
#endif
