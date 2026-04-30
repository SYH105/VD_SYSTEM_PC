################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS_DSP/Source/arm_bitreversal2.c \
../Drivers/CMSIS_DSP/Source/arm_cfft_f32.c \
../Drivers/CMSIS_DSP/Source/arm_cfft_init_f32.c \
../Drivers/CMSIS_DSP/Source/arm_cfft_radix8_f32.c \
../Drivers/CMSIS_DSP/Source/arm_common_tables.c \
../Drivers/CMSIS_DSP/Source/arm_const_structs.c \
../Drivers/CMSIS_DSP/Source/arm_rfft_fast_f32.c \
../Drivers/CMSIS_DSP/Source/arm_rfft_fast_init_f32.c 

OBJS += \
./Drivers/CMSIS_DSP/Source/arm_bitreversal2.o \
./Drivers/CMSIS_DSP/Source/arm_cfft_f32.o \
./Drivers/CMSIS_DSP/Source/arm_cfft_init_f32.o \
./Drivers/CMSIS_DSP/Source/arm_cfft_radix8_f32.o \
./Drivers/CMSIS_DSP/Source/arm_common_tables.o \
./Drivers/CMSIS_DSP/Source/arm_const_structs.o \
./Drivers/CMSIS_DSP/Source/arm_rfft_fast_f32.o \
./Drivers/CMSIS_DSP/Source/arm_rfft_fast_init_f32.o 

C_DEPS += \
./Drivers/CMSIS_DSP/Source/arm_bitreversal2.d \
./Drivers/CMSIS_DSP/Source/arm_cfft_f32.d \
./Drivers/CMSIS_DSP/Source/arm_cfft_init_f32.d \
./Drivers/CMSIS_DSP/Source/arm_cfft_radix8_f32.d \
./Drivers/CMSIS_DSP/Source/arm_common_tables.d \
./Drivers/CMSIS_DSP/Source/arm_const_structs.d \
./Drivers/CMSIS_DSP/Source/arm_rfft_fast_f32.d \
./Drivers/CMSIS_DSP/Source/arm_rfft_fast_init_f32.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_DSP/Source/%.o Drivers/CMSIS_DSP/Source/%.su Drivers/CMSIS_DSP/Source/%.cyclo: ../Drivers/CMSIS_DSP/Source/%.c Drivers/CMSIS_DSP/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/App/SETTING" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/App/App" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/App/Processing" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/Fonts" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/CMSIS_DSP/Include" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/ADXL345" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/LCD" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/BSP" -I"C:/Users/YONG/STM32CubeIDE/workspace_1.18.0/VD_SYSTEM_PC/Drivers/Flash" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS_DSP-2f-Source

clean-Drivers-2f-CMSIS_DSP-2f-Source:
	-$(RM) ./Drivers/CMSIS_DSP/Source/arm_bitreversal2.cyclo ./Drivers/CMSIS_DSP/Source/arm_bitreversal2.d ./Drivers/CMSIS_DSP/Source/arm_bitreversal2.o ./Drivers/CMSIS_DSP/Source/arm_bitreversal2.su ./Drivers/CMSIS_DSP/Source/arm_cfft_f32.cyclo ./Drivers/CMSIS_DSP/Source/arm_cfft_f32.d ./Drivers/CMSIS_DSP/Source/arm_cfft_f32.o ./Drivers/CMSIS_DSP/Source/arm_cfft_f32.su ./Drivers/CMSIS_DSP/Source/arm_cfft_init_f32.cyclo ./Drivers/CMSIS_DSP/Source/arm_cfft_init_f32.d ./Drivers/CMSIS_DSP/Source/arm_cfft_init_f32.o ./Drivers/CMSIS_DSP/Source/arm_cfft_init_f32.su ./Drivers/CMSIS_DSP/Source/arm_cfft_radix8_f32.cyclo ./Drivers/CMSIS_DSP/Source/arm_cfft_radix8_f32.d ./Drivers/CMSIS_DSP/Source/arm_cfft_radix8_f32.o ./Drivers/CMSIS_DSP/Source/arm_cfft_radix8_f32.su ./Drivers/CMSIS_DSP/Source/arm_common_tables.cyclo ./Drivers/CMSIS_DSP/Source/arm_common_tables.d ./Drivers/CMSIS_DSP/Source/arm_common_tables.o ./Drivers/CMSIS_DSP/Source/arm_common_tables.su ./Drivers/CMSIS_DSP/Source/arm_const_structs.cyclo ./Drivers/CMSIS_DSP/Source/arm_const_structs.d ./Drivers/CMSIS_DSP/Source/arm_const_structs.o ./Drivers/CMSIS_DSP/Source/arm_const_structs.su ./Drivers/CMSIS_DSP/Source/arm_rfft_fast_f32.cyclo ./Drivers/CMSIS_DSP/Source/arm_rfft_fast_f32.d ./Drivers/CMSIS_DSP/Source/arm_rfft_fast_f32.o ./Drivers/CMSIS_DSP/Source/arm_rfft_fast_f32.su ./Drivers/CMSIS_DSP/Source/arm_rfft_fast_init_f32.cyclo ./Drivers/CMSIS_DSP/Source/arm_rfft_fast_init_f32.d ./Drivers/CMSIS_DSP/Source/arm_rfft_fast_init_f32.o ./Drivers/CMSIS_DSP/Source/arm_rfft_fast_init_f32.su

.PHONY: clean-Drivers-2f-CMSIS_DSP-2f-Source

