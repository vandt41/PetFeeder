################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bsp/Src/communication.c \
../bsp/Src/ds3231.c \
../bsp/Src/lcd.c \
../bsp/Src/servo.c 

OBJS += \
./bsp/Src/communication.o \
./bsp/Src/ds3231.o \
./bsp/Src/lcd.o \
./bsp/Src/servo.o 

C_DEPS += \
./bsp/Src/communication.d \
./bsp/Src/ds3231.d \
./bsp/Src/lcd.d \
./bsp/Src/servo.d 


# Each subdirectory must supply rules for building sources it contributes
bsp/Src/%.o bsp/Src/%.su bsp/Src/%.cyclo: ../bsp/Src/%.c bsp/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"/home/brianmoser/Documents/projects/PetFeeder/app/Inc" -I"/home/brianmoser/Documents/projects/PetFeeder/bsp/Inc" -I"/home/brianmoser/Documents/projects/PetFeeder/Core/Inc" -I"/home/brianmoser/Documents/projects/PetFeeder/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-bsp-2f-Src

clean-bsp-2f-Src:
	-$(RM) ./bsp/Src/communication.cyclo ./bsp/Src/communication.d ./bsp/Src/communication.o ./bsp/Src/communication.su ./bsp/Src/ds3231.cyclo ./bsp/Src/ds3231.d ./bsp/Src/ds3231.o ./bsp/Src/ds3231.su ./bsp/Src/lcd.cyclo ./bsp/Src/lcd.d ./bsp/Src/lcd.o ./bsp/Src/lcd.su ./bsp/Src/servo.cyclo ./bsp/Src/servo.d ./bsp/Src/servo.o ./bsp/Src/servo.su

.PHONY: clean-bsp-2f-Src

