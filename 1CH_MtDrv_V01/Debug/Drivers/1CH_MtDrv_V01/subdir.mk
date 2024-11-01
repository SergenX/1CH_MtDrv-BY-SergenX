################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/1CH_MtDrv_V01/App.c 

OBJS += \
./Drivers/1CH_MtDrv_V01/App.o 

C_DEPS += \
./Drivers/1CH_MtDrv_V01/App.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/1CH_MtDrv_V01/%.o Drivers/1CH_MtDrv_V01/%.su Drivers/1CH_MtDrv_V01/%.cyclo: ../Drivers/1CH_MtDrv_V01/%.c Drivers/1CH_MtDrv_V01/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F042x6 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-1CH_MtDrv_V01

clean-Drivers-2f-1CH_MtDrv_V01:
	-$(RM) ./Drivers/1CH_MtDrv_V01/App.cyclo ./Drivers/1CH_MtDrv_V01/App.d ./Drivers/1CH_MtDrv_V01/App.o ./Drivers/1CH_MtDrv_V01/App.su

.PHONY: clean-Drivers-2f-1CH_MtDrv_V01

