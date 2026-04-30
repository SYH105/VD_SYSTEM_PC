################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/button.c \
../Drivers/BSP/buzzer.c \
../Drivers/BSP/graph.c 

OBJS += \
./Drivers/BSP/button.o \
./Drivers/BSP/buzzer.o \
./Drivers/BSP/graph.o 

C_DEPS += \
./Drivers/BSP/button.d \
./Drivers/BSP/buzzer.d \
./Drivers/BSP/graph.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/%.o Drivers/BSP/%.su Drivers/BSP/%.cyclo: ../Drivers/BSP/%.c Drivers/BSP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/App/SETTING" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/App/App" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/App/Processing" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/Fonts" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/CMSIS_DSP/Include" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/ADXL345" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/LCD" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/BSP" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/Flash" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP

clean-Drivers-2f-BSP:
	-$(RM) ./Drivers/BSP/button.cyclo ./Drivers/BSP/button.d ./Drivers/BSP/button.o ./Drivers/BSP/button.su ./Drivers/BSP/buzzer.cyclo ./Drivers/BSP/buzzer.d ./Drivers/BSP/buzzer.o ./Drivers/BSP/buzzer.su ./Drivers/BSP/graph.cyclo ./Drivers/BSP/graph.d ./Drivers/BSP/graph.o ./Drivers/BSP/graph.su

.PHONY: clean-Drivers-2f-BSP

