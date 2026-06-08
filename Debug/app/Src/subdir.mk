################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/Src/feeder.c 

OBJS += \
./app/Src/feeder.o 

C_DEPS += \
./app/Src/feeder.d 


# Each subdirectory must supply rules for building sources it contributes
app/Src/%.o app/Src/%.su app/Src/%.cyclo: ../app/Src/%.c app/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"/home/brianmoser/Documents/projects/PetFeeder/app/Inc" -I"/home/brianmoser/Documents/projects/PetFeeder/bsp/Inc" -I"/home/brianmoser/Documents/projects/PetFeeder/Core/Inc" -I"/home/brianmoser/Documents/projects/PetFeeder/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-app-2f-Src

clean-app-2f-Src:
	-$(RM) ./app/Src/feeder.cyclo ./app/Src/feeder.d ./app/Src/feeder.o ./app/Src/feeder.su

.PHONY: clean-app-2f-Src

