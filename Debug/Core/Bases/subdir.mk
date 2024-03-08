################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Bases/bases.cpp 

OBJS += \
./Core/Bases/bases.o 

CPP_DEPS += \
./Core/Bases/bases.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Bases/%.o Core/Bases/%.su Core/Bases/%.cyclo: ../Core/Bases/%.cpp Core/Bases/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m0 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F072xB -c -I../Core/Inc -I../Core/Drv_PIT -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Bases

clean-Core-2f-Bases:
	-$(RM) ./Core/Bases/bases.cyclo ./Core/Bases/bases.d ./Core/Bases/bases.o ./Core/Bases/bases.su

.PHONY: clean-Core-2f-Bases

