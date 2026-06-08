#ifndef FEEDER_H_
#define FEEDER_H_

#include "servo.h"

void Feeder_FeedOnce(void);
void Feeder_ButtonInit(void);
void Feeder_Run(void);
void delay_ms(uint32_t ms);

#endif
