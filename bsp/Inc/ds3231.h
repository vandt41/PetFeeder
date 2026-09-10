/*
 * ds3231.h
 *
 * Created on: Jul 02, 2026
 * Author: brianmoser
 */

#ifndef DS3231_H_
#define DS3231_H_

#include "stm32f407xx.h"
#include "gpio.h"
#include "i2c.h"
#include <stdbool.h>
#include <stdint.h>


/* =========================================================
 * Application configurable items
 * ========================================================= */

#define DS3231_I2C                  I2C1

#define DS3231_I2C_GPIO_PORT       GPIOB
#define DS3231_I2C_SDA_PIN         GPIO_PIN_NO_7
#define DS3231_I2C_SCL_PIN         GPIO_PIN_NO_6

#define DS3231_I2C_SPEED            I2C_SCL_SPEED_SM
#define DS3231_I2C_PUPD             GPIO_PIN_PU


/* =========================================================
 * DS3231 I2C Address
 * ========================================================= */

#define DS3231_I2C_ADDRESS          0x68


/* =========================================================
 * DS3231 Register Addresses
 * ========================================================= */

#define DS3231_REG_SECONDS          0x00
#define DS3231_REG_MINUTES          0x01
#define DS3231_REG_HOURS            0x02
#define DS3231_REG_DAY              0x03
#define DS3231_REG_DATE             0x04
#define DS3231_REG_MONTH            0x05
#define DS3231_REG_YEAR             0x06

#define DS3231_REG_ALARM1_SEC       0x07
#define DS3231_REG_ALARM1_MIN       0x08
#define DS3231_REG_ALARM1_HOUR      0x09
#define DS3231_REG_ALARM1_DAY_DATE  0x0A

#define DS3231_REG_ALARM2_MIN       0x0B
#define DS3231_REG_ALARM2_HOUR      0x0C
#define DS3231_REG_ALARM2_DAY_DATE  0x0D

#define DS3231_REG_CONTROL          0x0E
#define DS3231_REG_STATUS           0x0F
#define DS3231_REG_AGING            0x10
#define DS3231_REG_TEMP_MSB         0x11
#define DS3231_REG_TEMP_LSB         0x12


/* =========================================================
 * DS3231 Status Register Bits
 * ========================================================= */

#define DS3231_OSF_BIT              7
#define DS3231_A2F_BIT              1
#define DS3231_A1F_BIT              0


/* =========================================================
 * DS3231 Control Register Bits
 * ========================================================= */

#define DS3231_INTCN_BIT            2
#define DS3231_A2IE_BIT             1
#define DS3231_A1IE_BIT             0


/* =========================================================
 * Days of Week
 * ========================================================= */

#define SUNDAY                      1
#define MONDAY                      2
#define TUESDAY                     3
#define WEDNESDAY                   4
#define THURSDAY                    5
#define FRIDAY                      6
#define SATURDAY                    7


/* =========================================================
 * Time Format
 * ========================================================= */

typedef enum
{
    DS3231_TIME_FORMAT_12H_AM,
    DS3231_TIME_FORMAT_12H_PM,
    DS3231_TIME_FORMAT_24H

} DS3231_TimeFormat_t;


/* =========================================================
 * Date Structure
 * ========================================================= */

typedef struct
{
    uint8_t date;
    uint8_t month;
    uint8_t year;
    uint8_t day;

} DS3231_Date_t;


/* =========================================================
 * Time Structure
 * ========================================================= */

typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;

    DS3231_TimeFormat_t time_format;

} DS3231_Time_t;


/* =========================================================
 * Function Prototypes
 * ========================================================= */

void DS3231_Init(void);

void DS3231_SetTime(DS3231_Time_t *time);
void DS3231_GetTime(DS3231_Time_t *time);

void DS3231_SetDate(DS3231_Date_t *date);
void DS3231_GetDate(DS3231_Date_t *date);

void DS3231_GetDateTime(DS3231_Time_t *time,
                        DS3231_Date_t *date);

bool DS3231_TimeIsValid(void);
void DS3231_ClearOSF(void);

void DS3231_SetUnixTime(uint32_t unixTime);


/* Alarm functions */

void DS3231_AlarmEnable(void);
void DS3231_AlarmClear(void);
bool DS3231_AlarmTriggered(void);


#endif /* DS3231_H_ */
