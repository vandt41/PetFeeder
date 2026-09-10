/*
 * ds3231.c
 *
 * Created on: Jul 02, 2026
 * Author: brianmoser
 */

#include "ds3231.h"
#include <string.h>


/* =========================================================
 * Private Functions
 * ========================================================= */

static void DS3231_I2CPinConfig(void);
static void DS3231_I2CConfig(void);

static uint8_t DS3231_Read(uint8_t regAddr);
static void DS3231_Write(uint8_t value, uint8_t regAddr);

static uint8_t DS3231_BCDToBinary(uint8_t value);
static uint8_t DS3231_BinaryToBCD(uint8_t value);


/* =========================================================
 * Private Variables
 * ========================================================= */

static I2C_Handle_t g_ds3231I2CHandle;


/* =========================================================
 * Public Functions
 * ========================================================= */

void DS3231_Init(void)
{
    /* 1. Configure I2C GPIO pins */
    DS3231_I2CPinConfig();

    /* 2. Configure I2C peripheral */
    DS3231_I2CConfig();

    /* 3. Enable I2C peripheral */
    I2C_PeripheralControl(DS3231_I2C, ENABLE);
}


/* =========================================================
 * Set Time
 * ========================================================= */

void DS3231_SetTime(DS3231_Time_t *time)
{
    uint8_t hours;

    if (time == NULL)
    {
        return;
    }

    /* Seconds */
    DS3231_Write(
        DS3231_BinaryToBCD(time->seconds),
        DS3231_REG_SECONDS
    );

    /* Minutes */
    DS3231_Write(
        DS3231_BinaryToBCD(time->minutes),
        DS3231_REG_MINUTES
    );


    /* Hours */

    hours = DS3231_BinaryToBCD(time->hours);

    if (time->time_format == DS3231_TIME_FORMAT_24H)
    {
        /*
         * Bit 6 = 0 -> 24-hour mode
         */
        hours &= ~(1 << 6);
    }
    else
    {
        /*
         * Bit 6 = 1 -> 12-hour mode
         */
        hours |= (1 << 6);

        /*
         * Bit 5:
         * 0 -> AM
         * 1 -> PM
         */
        if (time->time_format == DS3231_TIME_FORMAT_12H_PM)
        {
            hours |= (1 << 5);
        }
        else
        {
            hours &= ~(1 << 5);
        }
    }

    DS3231_Write(
        hours,
        DS3231_REG_HOURS
    );
}


/* =========================================================
 * Set Date
 * ========================================================= */

void DS3231_SetDate(DS3231_Date_t *date)
{
    if (date == NULL)
    {
        return;
    }

    DS3231_Write(
        DS3231_BinaryToBCD(date->date),
        DS3231_REG_DATE
    );

    DS3231_Write(
        DS3231_BinaryToBCD(date->month),
        DS3231_REG_MONTH
    );

    DS3231_Write(
        DS3231_BinaryToBCD(date->year),
        DS3231_REG_YEAR
    );

    DS3231_Write(
        DS3231_BinaryToBCD(date->day),
        DS3231_REG_DAY
    );
}


/* =========================================================
 * Get Time
 * ========================================================= */

void DS3231_GetTime(DS3231_Time_t *time)
{
    uint8_t hours;

    if (time == NULL)
    {
        return;
    }

    /* Seconds */
    time->seconds =
        DS3231_BCDToBinary(
            DS3231_Read(DS3231_REG_SECONDS) & 0x7F
        );

    /* Minutes */
    time->minutes =
        DS3231_BCDToBinary(
            DS3231_Read(DS3231_REG_MINUTES)
        );


    /* Hours */
    hours = DS3231_Read(DS3231_REG_HOURS);


    if (hours & (1 << 6))
    {
        /*
         * 12-hour mode
         */

        if (hours & (1 << 5))
        {
            time->time_format =
                DS3231_TIME_FORMAT_12H_PM;
        }
        else
        {
            time->time_format =
                DS3231_TIME_FORMAT_12H_AM;
        }

        /*
         * Clear bit 6 and bit 5.
         */
        hours &= ~(0x03 << 5);

        time->hours =
            DS3231_BCDToBinary(hours);
    }
    else
    {
        /*
         * 24-hour mode
         */

        time->time_format =
            DS3231_TIME_FORMAT_24H;

        time->hours =
            DS3231_BCDToBinary(
                hours & 0x3F
            );
    }
}


/* =========================================================
 * Get Date
 * ========================================================= */

void DS3231_GetDate(DS3231_Date_t *date)
{
    if (date == NULL)
    {
        return;
    }

    date->day =
        DS3231_BCDToBinary(
            DS3231_Read(DS3231_REG_DAY)
        );

    date->date =
        DS3231_BCDToBinary(
            DS3231_Read(DS3231_REG_DATE)
        );

    date->month =
        DS3231_BCDToBinary(
            DS3231_Read(DS3231_REG_MONTH) & 0x1F
        );

    date->year =
        DS3231_BCDToBinary(
            DS3231_Read(DS3231_REG_YEAR)
        );
}


/* =========================================================
 * Get Date + Time
 * ========================================================= */

void DS3231_GetDateTime(DS3231_Time_t *time,
                        DS3231_Date_t *date)
{
    uint8_t buffer[7];
    uint8_t reg = DS3231_REG_SECONDS;

    if ((time == NULL) || (date == NULL))
    {
        return;
    }


    /*
     * Set register pointer to seconds register.
     */
    I2C_MasterSendData(
        &g_ds3231I2CHandle,
        &reg,
        1,
        DS3231_I2C_ADDRESS,
        I2C_ENABLE_SR
    );


    /*
     * Read seconds -> year
     * 7 consecutive registers.
     */
    I2C_MasterReceiveData(
        &g_ds3231I2CHandle,
        buffer,
        7,
        DS3231_I2C_ADDRESS,
        I2C_DISABLE_SR
    );


    /* Seconds */
    time->seconds =
        DS3231_BCDToBinary(
            buffer[0] & 0x7F
        );


    /* Minutes */
    time->minutes =
        DS3231_BCDToBinary(
            buffer[1]
        );


    /* Hours */
    if (buffer[2] & (1 << 6))
    {
        /*
         * 12-hour mode
         */

        if (buffer[2] & (1 << 5))
        {
            time->time_format =
                DS3231_TIME_FORMAT_12H_PM;
        }
        else
        {
            time->time_format =
                DS3231_TIME_FORMAT_12H_AM;
        }

        time->hours =
            DS3231_BCDToBinary(
                buffer[2] & 0x1F
            );
    }
    else
    {
        /*
         * 24-hour mode
         */

        time->time_format =
            DS3231_TIME_FORMAT_24H;

        time->hours =
            DS3231_BCDToBinary(
                buffer[2] & 0x3F
            );
    }


    /* Date */
    date->day =
        DS3231_BCDToBinary(buffer[3]);

    date->date =
        DS3231_BCDToBinary(buffer[4]);

    date->month =
        DS3231_BCDToBinary(buffer[5] & 0x1F);

    date->year =
        DS3231_BCDToBinary(buffer[6]);
}


/* =========================================================
 * RTC Validity
 * ========================================================= */

bool DS3231_TimeIsValid(void)
{
    uint8_t status;

    status = DS3231_Read(
        DS3231_REG_STATUS
    );

    /*
     * OSF = 1 means oscillator stopped
     * at some point.
     */
    return ((status & (1 << DS3231_OSF_BIT)) == 0);
}


/* =========================================================
 * Clear Oscillator Stop Flag
 * ========================================================= */

void DS3231_ClearOSF(void)
{
    uint8_t status;

    status = DS3231_Read(
        DS3231_REG_STATUS
    );

    status &= ~(1 << DS3231_OSF_BIT);

    DS3231_Write(
        status,
        DS3231_REG_STATUS
    );
}


/* =========================================================
 * Set Unix Time
 * ========================================================= */

void DS3231_SetUnixTime(uint32_t unixTime)
{
    DS3231_Time_t time;
    DS3231_Date_t date;

    uint32_t days;
    uint32_t seconds;

    int32_t z;
    int32_t era;
    uint32_t doe;
    uint32_t yoe;
    int32_t y;
    uint32_t doy;
    uint32_t mp;


    /*
     * Unix timestamp:
     *
     * 1970-01-01 00:00:00
     */

    days = unixTime / 86400;
    seconds = unixTime % 86400;


    /* =====================================================
     * Convert time
     * ===================================================== */

    time.hours = seconds / 3600;

    seconds %= 3600;

    time.minutes = seconds / 60;

    time.seconds = seconds % 60;

    time.time_format =
        DS3231_TIME_FORMAT_24H;


    /* =====================================================
     * Convert date
     * ===================================================== */

    z = days + 719468;

    era =
        (z >= 0 ? z : z - 146096)
        / 146097;

    doe =
        z - era * 146097;

    yoe =
        (doe
        - doe / 1460
        + doe / 36524
        - doe / 146096)
        / 365;

    y =
        yoe + era * 400;

    doy =
        doe
        - (365 * yoe
        + yoe / 4
        - yoe / 100);

    mp =
        (5 * doy + 2) / 153;


    date.date =
        doy
        - (153 * mp + 2) / 5
        + 1;

    date.month =
        mp + (mp < 10 ? 3 : -9);

    date.year =
        (date.month <= 2)
        ? (y + 1 - 2000)
        : (y - 2000);


    /*
     * Day of week
     *
     * 1 = Sunday
     * 2 = Monday
     * ...
     * 7 = Saturday
     */

    date.day =
        ((days + 4) % 7) + 1;


    /* Write RTC */
    DS3231_SetTime(&time);

    DS3231_SetDate(&date);

    DS3231_ClearOSF();
}


/* =========================================================
 * Alarm
 * ========================================================= */

void DS3231_AlarmEnable(void)
{
    uint8_t control;

    control =
        DS3231_Read(
            DS3231_REG_CONTROL
        );

    /*
     * Enable interrupt control.
     */
    control |=
        (1 << DS3231_INTCN_BIT);

    /*
     * Enable Alarm 1 interrupt.
     */
    control |=
        (1 << DS3231_A1IE_BIT);

    DS3231_Write(
        control,
        DS3231_REG_CONTROL
    );
}


/* =========================================================
 * Clear Alarm
 * ========================================================= */

void DS3231_AlarmClear(void)
{
    uint8_t status;

    status =
        DS3231_Read(
            DS3231_REG_STATUS
        );

    /*
     * Clear Alarm 1 flag.
     */
    status &=
        ~(1 << DS3231_A1F_BIT);

    DS3231_Write(
        status,
        DS3231_REG_STATUS
    );
}


/* =========================================================
 * Check Alarm
 * ========================================================= */

bool DS3231_AlarmTriggered(void)
{
    uint8_t status;

    status =
        DS3231_Read(
            DS3231_REG_STATUS
        );

    return ((status &
            (1 << DS3231_A1F_BIT)) != 0);
}


/* =========================================================
 * I2C GPIO Configuration
 * ========================================================= */

static void DS3231_I2CPinConfig(void)
{
    GPIO_Handle_t i2c_sda;
    GPIO_Handle_t i2c_scl;


    memset(
        &i2c_sda,
        0,
        sizeof(i2c_sda)
    );

    memset(
        &i2c_scl,
        0,
        sizeof(i2c_scl)
    );


    /*
     * I2C1_SDA -> PB7
     */

    i2c_sda.pGPIOx =
        DS3231_I2C_GPIO_PORT;

    i2c_sda.GPIO_PinConfig.GPIO_PinAltFunMode =
        4;

    i2c_sda.GPIO_PinConfig.GPIO_PinMode =
        GPIO_MODE_ALTFN;

    i2c_sda.GPIO_PinConfig.GPIO_PinNumber =
        DS3231_I2C_SDA_PIN;

    i2c_sda.GPIO_PinConfig.GPIO_PinOPType =
        GPIO_OP_TYPE_OD;

    i2c_sda.GPIO_PinConfig.GPIO_PinPuPdControl =
        DS3231_I2C_PUPD;

    i2c_sda.GPIO_PinConfig.GPIO_PinSpeed =
        GPIO_SPEED_FAST;

    GPIO_Init(&i2c_sda);


    /*
     * I2C1_SCL -> PB6
     */

    i2c_scl.pGPIOx =
        DS3231_I2C_GPIO_PORT;

    i2c_scl.GPIO_PinConfig.GPIO_PinAltFunMode =
        4;

    i2c_scl.GPIO_PinConfig.GPIO_PinMode =
        GPIO_MODE_ALTFN;

    i2c_scl.GPIO_PinConfig.GPIO_PinNumber =
        DS3231_I2C_SCL_PIN;

    i2c_scl.GPIO_PinConfig.GPIO_PinOPType =
        GPIO_OP_TYPE_OD;

    i2c_scl.GPIO_PinConfig.GPIO_PinPuPdControl =
        DS3231_I2C_PUPD;

    i2c_scl.GPIO_PinConfig.GPIO_PinSpeed =
        GPIO_SPEED_FAST;

    GPIO_Init(&i2c_scl);
}


/* =========================================================
 * I2C Peripheral Configuration
 * ========================================================= */

static void DS3231_I2CConfig(void)
{
    g_ds3231I2CHandle.pI2Cx =
        DS3231_I2C;

    g_ds3231I2CHandle.I2C_Config.I2C_AckControl =
        I2C_ACK_ENABLE;

    g_ds3231I2CHandle.I2C_Config.I2C_SCLSpeed =
        DS3231_I2C_SPEED;

    I2C_Init(
        &g_ds3231I2CHandle
    );
}


/* =========================================================
 * Write One DS3231 Register
 * ========================================================= */

static void DS3231_Write(uint8_t value,
                         uint8_t regAddr)
{
    uint8_t tx[2];

    tx[0] = regAddr;
    tx[1] = value;

    I2C_MasterSendData(
        &g_ds3231I2CHandle,
        tx,
        2,
        DS3231_I2C_ADDRESS,
        0
    );
}


/* =========================================================
 * Read One DS3231 Register
 * ========================================================= */

static uint8_t DS3231_Read(uint8_t regAddr)
{
    uint8_t data;

    /*
     * Set DS3231 register pointer.
     */
    I2C_MasterSendData(
        &g_ds3231I2CHandle,
        &regAddr,
        1,
        DS3231_I2C_ADDRESS,
        0
    );


    /*
     * Read register.
     */
    I2C_MasterReceiveData(
        &g_ds3231I2CHandle,
        &data,
        1,
        DS3231_I2C_ADDRESS,
        0
    );


    return data;
}


/* =========================================================
 * Binary -> BCD
 * ========================================================= */

static uint8_t DS3231_BinaryToBCD(uint8_t value)
{
    uint8_t tens;
    uint8_t ones;

    tens = value / 10;
    ones = value % 10;

    return ((tens << 4) | ones);
}


/* =========================================================
 * BCD -> Binary
 * ========================================================= */

static uint8_t DS3231_BCDToBinary(uint8_t value)
{
    uint8_t tens;
    uint8_t ones;

    tens = (value >> 4) * 10;
    ones = value & 0x0F;

    return (tens + ones);
}
