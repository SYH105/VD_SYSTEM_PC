################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/LCD/ili9341.c \
../Drivers/LCD/lcd.c \
../Drivers/LCD/lcd_text.c \
../Drivers/LCD/ui.c 

OBJS += \
./Drivers/LCD/ili9341.o \
./Drivers/LCD/lcd.o \
./Drivers/LCD/lcd_text.o \
./Drivers/LCD/ui.o 

C_DEPS += \
./Drivers/LCD/ili9341.d \
./Drivers/LCD/lcd.d \
./Drivers/LCD/lcd_text.d \
./Drivers/LCD/ui.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/LCD/%.o Drivers/LCD/%.su Drivers/LCD/%.cyclo: ../Drivers/LCD/%.c Drivers/LCD/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/App/SETTING" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/App/App" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/App/Processing" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/Fonts" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/CMSIS_DSP/Include" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/ADXL345" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/LCD" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/BSP" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/Flash" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-LCD

clean-Drivers-2f-LCD:
	-$(RM) ./Drivers/LCD/ili9341.cyclo ./Drivers/LCD/ili9341.d ./Drivers/LCD/ili9341.o ./Drivers/LCD/ili9341.su ./Drivers/LCD/lcd.cyclo ./Drivers/LCD/lcd.d ./Drivers/LCD/lcd.o ./Drivers/LCD/lcd.su ./Drivers/LCD/lcd_text.cyclo ./Drivers/LCD/lcd_text.d ./Drivers/LCD/lcd_text.o ./Drivers/LCD/lcd_text.su ./Drivers/LCD/ui.cyclo ./Drivers/LCD/ui.d ./Drivers/LCD/ui.o ./Drivers/LCD/ui.su

.PHONY: clean-Drivers-2f-LCD

