################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/User_App/Memory/user_mem_eeprom_data.c \
../Core/Src/User_App/Memory/user_mem_eeprom_system.c \
../Core/Src/User_App/Memory/user_mem_sdram.c 

C_DEPS += \
./Core/Src/User_App/Memory/user_mem_eeprom_data.d \
./Core/Src/User_App/Memory/user_mem_eeprom_system.d \
./Core/Src/User_App/Memory/user_mem_sdram.d 

OBJS += \
./Core/Src/User_App/Memory/user_mem_eeprom_data.o \
./Core/Src/User_App/Memory/user_mem_eeprom_system.o \
./Core/Src/User_App/Memory/user_mem_sdram.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/User_App/Memory/%.o Core/Src/User_App/Memory/%.su Core/Src/User_App/Memory/%.cyclo: ../Core/Src/User_App/Memory/%.c Core/Src/User_App/Memory/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../TouchGFX/App -I../TouchGFX/target/generated -I../TouchGFX/target -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Utilities/JPEG -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/touchgfx/framework/include -I../TouchGFX/generated/fonts/include -I../TouchGFX/generated/gui_generated/include -I../TouchGFX/generated/images/include -I../TouchGFX/generated/texts/include -I../TouchGFX/generated/videos/include -I../TouchGFX/gui/include -I"C:/Users/user/GitHub/Display_TouchGFX_OWLAB/Display_Creation_TEST/Core/Inc/BSP/Font" -I"C:/Users/user/GitHub/Display_TouchGFX_OWLAB/Display_Creation_TEST/Core/Inc/BSP/STM32H7_BSP" -I"C:/Users/user/GitHub/Display_TouchGFX_OWLAB/Display_Creation_TEST/Core/Inc/User_App/Display" -I"C:/Users/user/GitHub/Display_TouchGFX_OWLAB/Display_Creation_TEST/Core/Inc/User_App/General" -I"C:/Users/user/GitHub/Display_TouchGFX_OWLAB/Display_Creation_TEST/Core/Inc/User_App/Memory" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-User_App-2f-Memory

clean-Core-2f-Src-2f-User_App-2f-Memory:
	-$(RM) ./Core/Src/User_App/Memory/user_mem_eeprom_data.cyclo ./Core/Src/User_App/Memory/user_mem_eeprom_data.d ./Core/Src/User_App/Memory/user_mem_eeprom_data.o ./Core/Src/User_App/Memory/user_mem_eeprom_data.su ./Core/Src/User_App/Memory/user_mem_eeprom_system.cyclo ./Core/Src/User_App/Memory/user_mem_eeprom_system.d ./Core/Src/User_App/Memory/user_mem_eeprom_system.o ./Core/Src/User_App/Memory/user_mem_eeprom_system.su ./Core/Src/User_App/Memory/user_mem_sdram.cyclo ./Core/Src/User_App/Memory/user_mem_sdram.d ./Core/Src/User_App/Memory/user_mem_sdram.o ./Core/Src/User_App/Memory/user_mem_sdram.su

.PHONY: clean-Core-2f-Src-2f-User_App-2f-Memory

