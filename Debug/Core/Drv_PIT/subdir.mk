################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Drv_PIT/drv_pit.cpp 

OBJS += \
./Core/Drv_PIT/drv_pit.o 

CPP_DEPS += \
./Core/Drv_PIT/drv_pit.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Drv_PIT/%.o Core/Drv_PIT/%.su Core/Drv_PIT/%.cyclo: ../Core/Drv_PIT/%.cpp Core/Drv_PIT/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m0 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F072xB -c -I../Core/Inc -I../Core/Drv_PIT -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Drv_PIT

clean-Core-2f-Drv_PIT:
	-$(RM) ./Core/Drv_PIT/drv_pit.cyclo ./Core/Drv_PIT/drv_pit.d ./Core/Drv_PIT/drv_pit.o ./Core/Drv_PIT/drv_pit.su

.PHONY: clean-Core-2f-Drv_PIT

