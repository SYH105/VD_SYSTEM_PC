################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Processing/fft_process.c 

OBJS += \
./App/Processing/fft_process.o 

C_DEPS += \
./App/Processing/fft_process.d 


# Each subdirectory must supply rules for building sources it contributes
App/Processing/%.o App/Processing/%.su App/Processing/%.cyclo: ../App/Processing/%.c App/Processing/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/App/SETTING" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/App/App" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/App/Processing" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/Fonts" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/CMSIS_DSP/Include" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/ADXL345" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/LCD" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/BSP" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/Flash" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Processing

clean-App-2f-Processing:
	-$(RM) ./App/Processing/fft_process.cyclo ./App/Processing/fft_process.d ./App/Processing/fft_process.o ./App/Processing/fft_process.su

.PHONY: clean-App-2f-Processing

