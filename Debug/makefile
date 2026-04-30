################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Drivers/LCD/subdir.mk
-include Drivers/Fonts/subdir.mk
-include Drivers/Flash/subdir.mk
-include Drivers/CMSIS_DSP/Source/subdir.mk
-include Drivers/BSP/subdir.mk
-include Drivers/ADXL345/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include App/SETTING/subdir.mk
-include App/Processing/subdir.mk
-include App/App/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := VD_SYSTEM_PC
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
VD_SYSTEM_PC.elf \

MAP_FILES += \
VD_SYSTEM_PC.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
VD_SYSTEM_PC.list \

OBJCOPY_HEX += \
VD_SYSTEM_PC.hex \


# All Target
all: main-build

# Main-build Target
main-build: VD_SYSTEM_PC.elf secondary-outputs

# Tool invocations
VD_SYSTEM_PC.elf VD_SYSTEM_PC.map: $(OBJS) $(USER_OBJS) C:\Users\YONG\STM32CubeIDE\workspace_1.18.0\VD_SYSTEM_PC\STM32F407VETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "VD_SYSTEM_PC.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\YONG\STM32CubeIDE\workspace_1.18.0\VD_SYSTEM_PC\STM32F407VETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="VD_SYSTEM_PC.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

VD_SYSTEM_PC.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "VD_SYSTEM_PC.list"
	@echo 'Finished building: $@'
	@echo ' '

VD_SYSTEM_PC.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "VD_SYSTEM_PC.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) VD_SYSTEM_PC.elf VD_SYSTEM_PC.hex VD_SYSTEM_PC.list VD_SYSTEM_PC.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
