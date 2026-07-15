/*
 * ds3231.h
 *
 *  Created on: Jul 02, 2026
 *      Author: brianmoser
 */
#ifndef DS3231_H
#define DS3231_H

#include "stm32f407xx.h"
#include "gpio.h"
#include "i2c.h"
#include <stdbool.h>

/*Application configurable items */
#define DS3231_I2C  			I2C1
#define DS3231_I2C_GPIO_PORT    GPIOB
#define DS3231_I2C_SDA_PIN 		GPIO_PIN_NO_7
#define DS3231_I2C_SCL_PIN 		GPIO_PIN_NO_6
#define DS3231_I2C_SPEED 		I2C_SCL_SPEED_SM
#define DS3231_I2C_PUPD			GPIO_PIN_PU

/*Register addresses */
#define DS3231_ADDR_SEC 		0x00
#define DS3231_ADDR_MIN 		0x01
#define DS3231_ADDR_HRS			0x02
#define DS3231_ADDR_DAY			0x03
#define DS3231_ADDR_DATE		0x04
#define DS3231_ADDR_MONTH		0x05
#define DS3231_ADDR_YEAR		0x06
#define DS3231_ADDR_STATUS    	0x0F

#define DS3231_OSF_BIT        	7

#define TIME_FORMAT_12HRS_AM 	0
#define TIME_FORMAT_12HRS_PM 	1
#define TIME_FORMAT_24HRS 		2

#define DS3231_I2C_ADDRESS      0x68


#define SUNDAY  	1;
#define MONDAY  	2;
#define TUESDAY  	3;
#define WEDNESDAY   4;
#define THURSDAY  	5;
#define FRIDAY  	6;
#define SATURDAY  	7;


typedef struct
{
	uint8_t date;
	uint8_t month;
	uint8_t year;
	uint8_t day;
}RTC_date_t;


typedef struct
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t time_format;
}RTC_time_t;



//Function prototypes

void ds3231_init(void);

void ds3231_set_current_time(RTC_time_t *);
void ds3231_get_current_time(RTC_time_t *);

void ds3231_set_current_date(RTC_date_t *);
void ds3231_get_current_date(RTC_date_t *);

bool ds3231_time_valid(void);
void ds3231_clear_osf(void);

void ds3231_set_unix_time(uint32_t unixTime);
#endif
