################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/Src/gpio.c \
../drivers/Src/i2c.c \
../drivers/Src/rcc.c \
../drivers/Src/spi.c \
../drivers/Src/usart.c 

OBJS += \
./drivers/Src/gpio.o \
./drivers/Src/i2c.o \
./drivers/Src/rcc.o \
./drivers/Src/spi.o \
./drivers/Src/usart.o 

C_DEPS += \
./drivers/Src/gpio.d \
./drivers/Src/i2c.d \
./drivers/Src/rcc.d \
./drivers/Src/spi.d \
./drivers/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/Src/%.o drivers/Src/%.su drivers/Src/%.cyclo: ../drivers/Src/%.c drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"/home/brianmoser/Documents/projects/PetFeeder/app/Inc" -I"/home/brianmoser/Documents/projects/PetFeeder/bsp/Inc" -I"/home/brianmoser/Documents/projects/PetFeeder/Core/Inc" -I"/home/brianmoser/Documents/projects/PetFeeder/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-drivers-2f-Src

clean-drivers-2f-Src:
	-$(RM) ./drivers/Src/gpio.cyclo ./drivers/Src/gpio.d ./drivers/Src/gpio.o ./drivers/Src/gpio.su ./drivers/Src/i2c.cyclo ./drivers/Src/i2c.d ./drivers/Src/i2c.o ./drivers/Src/i2c.su ./drivers/Src/rcc.cyclo ./drivers/Src/rcc.d ./drivers/Src/rcc.o ./drivers/Src/rcc.su ./drivers/Src/spi.cyclo ./drivers/Src/spi.d ./drivers/Src/spi.o ./drivers/Src/spi.su ./drivers/Src/usart.cyclo ./drivers/Src/usart.d ./drivers/Src/usart.o ./drivers/Src/usart.su

.PHONY: clean-drivers-2f-Src

