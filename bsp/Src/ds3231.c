
#include<stdint.h>
#include<string.h>

#include "ds3231.h"

static void ds3231_i2c_pin_config(void);
static void ds3231_i2c_config(void);
static uint8_t ds3231_read(uint8_t reg_addr);
static void ds3231_write(uint8_t value,uint8_t reg_addr);
static uint8_t bcd_to_binary(uint8_t value);
static uint8_t binary_to_bcd(uint8_t value);

I2C_Handle_t g_ds3231I2cHandle;

void ds3231_init(void)
{

	//1. init the i2c pins
	ds3231_i2c_pin_config();

	//2. initialize the i2c peripheral
	ds3231_i2c_config();

	//3. Enable the I2C peripheral
	I2C_PeripheralControl(DS3231_I2C, ENABLE);
}


void ds3231_set_current_time(RTC_time_t *rtc_time)
{
	uint8_t hrs;
	ds3231_write(binary_to_bcd(rtc_time->seconds), DS3231_ADDR_SEC);

	ds3231_write(binary_to_bcd(rtc_time->minutes), DS3231_ADDR_MIN);

	hrs = binary_to_bcd(rtc_time->hours);

	if(rtc_time->time_format == TIME_FORMAT_24HRS){
		hrs &= ~(1 << 6);
	}else{
		hrs |= (1 << 6);
		hrs = (rtc_time->time_format  == TIME_FORMAT_12HRS_PM) ? hrs | ( 1 << 5) :  hrs & ~( 1 << 5) ;
	}

	ds3231_write(hrs,DS3231_ADDR_HRS);

}

void ds3231_set_current_date(RTC_date_t *rtc_date)
{
	ds3231_write(binary_to_bcd(rtc_date->date),DS3231_ADDR_DATE);

	ds3231_write(binary_to_bcd(rtc_date->month),DS3231_ADDR_MONTH);

	ds3231_write(binary_to_bcd(rtc_date->year),DS3231_ADDR_YEAR);

	ds3231_write(binary_to_bcd(rtc_date->day),DS3231_ADDR_DAY);

}
void ds3231_get_datetime(RTC_time_t *time, RTC_date_t *date)
{
    uint8_t buffer[7];
    uint8_t reg = DS3231_ADDR_SEC;

    /* Set register pointer */
    I2C_MasterSendData(&g_ds3231I2cHandle,
                       &reg,
                       1,
                       DS3231_I2C_ADDRESS,
                       I2C_ENABLE_SR);

    /* Burst read 7 registers */
    I2C_MasterReceiveData(&g_ds3231I2cHandle,
                          buffer,
                          7,
                          DS3231_I2C_ADDRESS,
                          I2C_DISABLE_SR);

    time->seconds = bcd_to_binary(buffer[0] & 0x7F);
    time->minutes = bcd_to_binary(buffer[1]);

    if(buffer[2] & (1 << 6))
    {
        time->time_format =
            (buffer[2] & (1 << 5)) ?
            TIME_FORMAT_12HRS_PM :
            TIME_FORMAT_12HRS_AM;

        time->hours =
            bcd_to_binary(buffer[2] & 0x1F);
    }
    else
    {
        time->time_format = TIME_FORMAT_24HRS;
        time->hours =
            bcd_to_binary(buffer[2] & 0x3F);
    }

    date->day   = bcd_to_binary(buffer[3]);
    date->date  = bcd_to_binary(buffer[4]);
    date->month = bcd_to_binary(buffer[5] & 0x1F);
    date->year  = bcd_to_binary(buffer[6]);
}
void ds3231_get_current_time(RTC_time_t *rtc_time)
{

	uint8_t hrs;

	rtc_time->seconds = bcd_to_binary(ds3231_read(DS3231_ADDR_SEC));
	rtc_time->minutes = bcd_to_binary(ds3231_read(DS3231_ADDR_MIN));

	hrs = ds3231_read(DS3231_ADDR_HRS);
	if(hrs & ( 1 << 6)){
		//12 hr format
		rtc_time->time_format =  !((hrs & ( 1 << 5)) == 0) ;
		hrs &= ~(0x3 << 5);//Clear 6 and 5
	}else{
		//24 hr format
		rtc_time->time_format = TIME_FORMAT_24HRS;
	}

	rtc_time->hours = bcd_to_binary(hrs);
}

void ds3231_get_current_date(RTC_date_t *rtc_date)
{
	rtc_date->day =  bcd_to_binary(ds3231_read(DS3231_ADDR_DAY));
	rtc_date->date = bcd_to_binary(ds3231_read(DS3231_ADDR_DATE));
	rtc_date->month = bcd_to_binary(ds3231_read(DS3231_ADDR_MONTH));
	rtc_date->year = bcd_to_binary(ds3231_read(DS3231_ADDR_YEAR));

}

static void ds3231_i2c_pin_config(void)
{
	GPIO_Handle_t i2c_sda,i2c_scl;

	memset(&i2c_sda,0,sizeof(i2c_sda));
	memset(&i2c_scl,0,sizeof(i2c_scl));

	/*
	 * I2C1_SCL ==> PB6
	 * I2C1_SDA ==> PB7
	 */

	i2c_sda.pGPIOx = DS3231_I2C_GPIO_PORT;
	i2c_sda.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	i2c_sda.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	i2c_sda.GPIO_PinConfig.GPIO_PinNumber = DS3231_I2C_SDA_PIN;
	i2c_sda.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	i2c_sda.GPIO_PinConfig.GPIO_PinPuPdControl = DS3231_I2C_PUPD;
	i2c_sda.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_Init(&i2c_sda);


	i2c_scl.pGPIOx = DS3231_I2C_GPIO_PORT;
	i2c_scl.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	i2c_scl.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	i2c_scl.GPIO_PinConfig.GPIO_PinNumber = DS3231_I2C_SCL_PIN;
	i2c_scl.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	i2c_scl.GPIO_PinConfig.GPIO_PinPuPdControl = DS3231_I2C_PUPD;
	i2c_scl.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_Init(&i2c_scl);

}


static void ds3231_i2c_config(void)
{
	g_ds3231I2cHandle.pI2Cx = DS3231_I2C;
	g_ds3231I2cHandle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
	g_ds3231I2cHandle.I2C_Config.I2C_SCLSpeed = DS3231_I2C_SPEED;
	I2C_Init(&g_ds3231I2cHandle);
}


static void ds3231_write(uint8_t value,uint8_t reg_addr)
{
	uint8_t tx[2];
	tx[0] = reg_addr;
	tx[1] = value;
	I2C_MasterSendData(&g_ds3231I2cHandle, tx, 2, DS3231_I2C_ADDRESS, 0);
}



static uint8_t ds3231_read(uint8_t reg_addr)
{
	uint8_t data;
    I2C_MasterSendData(&g_ds3231I2cHandle, &reg_addr, 1, DS3231_I2C_ADDRESS, 0);
    I2C_MasterReceiveData(&g_ds3231I2cHandle, &data, 1, DS3231_I2C_ADDRESS, 0);

    return data;
}

static uint8_t binary_to_bcd(uint8_t value)
{
	uint8_t m , n;
	uint8_t bcd;

	bcd = value;
	if(value >= 10)
	{
		m = value /10;
		n = value % 10;
		bcd = (m << 4) | n ;
	}

	return bcd;
}

static uint8_t bcd_to_binary(uint8_t value)
{
	uint8_t m , n;
	m = (uint8_t) ((value >> 4 ) * 10);
	n =  value & (uint8_t)0x0F;
	return (m+n);
}

bool ds3231_time_valid(void)
{
    uint8_t status = ds3231_read(DS3231_ADDR_STATUS);

    return ((status & (1 << DS3231_OSF_BIT)) == 0);
}
void ds3231_clear_osf(void)
{
    uint8_t status = ds3231_read(DS3231_ADDR_STATUS);

    status &= ~(1 << 7);

    ds3231_write(status, DS3231_ADDR_STATUS);
}
void ds3231_set_unix_time(uint32_t unixTime)
{
    RTC_time_t time;
    RTC_date_t date;

    uint32_t days = unixTime / 86400;
    uint32_t secs = unixTime % 86400;

    /* Time */
    time.hours = secs / 3600;
    secs %= 3600;

    time.minutes = secs / 60;
    time.seconds = secs % 60;

    time.time_format = TIME_FORMAT_24HRS;

    /* Date */
    int32_t z = days + 719468;
    int32_t era = (z >= 0 ? z : z - 146096) / 146097;
    uint32_t doe = z - era * 146097;
    uint32_t yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    int32_t y = yoe + era * 400;
    uint32_t doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    uint32_t mp = (5 * doy + 2) / 153;

    date.date = doy - (153 * mp + 2) / 5 + 1;
    date.month = mp + (mp < 10 ? 3 : -9);
    date.year = (date.month <= 2) ? (y + 1 - 2000) : (y - 2000);

    /* Day of week (1 = Sunday) */
    date.day = ((days + 4) % 7) + 1;

    ds3231_set_current_time(&time);
    ds3231_set_current_date(&date);

    ds3231_clear_osf();
}
void ds3231_alarm_enable(void)
{
    uint8_t ctrl;

    ctrl = ds3231_read(0x0E);

    ctrl |= (1 << 2);   // INTCN

    ctrl |= (1 << 0);   // Alarm1 enable

    ds3231_write(ctrl,0x0E);
}
void ds3231_alarm_clear(void)
{
    uint8_t status;

    status = ds3231_read(0x0F);

    status &= ~(1 << 0);

    ds3231_write(status,0x0F);
}
bool ds3231_alarm_triggered(void)
{
    uint8_t status;

    status = ds3231_read(0x0F);

    return status & 0x01;
}
