################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/BSP/STM32H7_BSP/stm32h743i_eval.c \
../Core/Src/BSP/STM32H7_BSP/stm32h743i_eval_eeprom.c \
../Core/Src/BSP/STM32H7_BSP/stm32h743i_eval_lcd.c 

C_DEPS += \
./Core/Src/BSP/STM32H7_BSP/stm32h743i_eval.d \
./Core/Src/BSP/STM32H7_BSP/stm32h743i_eval_eeprom.d \
./Core/Src/BSP/STM32H7_BSP/stm32h743i_eval_lcd.d 

OBJS += \
./Core/Src/BSP/STM32H7_BSP/stm32h743i_eval.o \
./Core/Src/BSP/STM32H7_BSP/stm32h743i_eval_eeprom.o \
./Core/Src/BSP/STM32H7_BSP/stm32h743i_eval_lcd.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/BSP/STM32H7_BSP/%.o Core/Src/BSP/STM32H7_BSP/%.su Core/Src/BSP/STM32H7_BSP/%.cyclo: ../Core/Src/BSP/STM32H7_BSP/%.c Core/Src/BSP/STM32H7_BSP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../TouchGFX/App -I../TouchGFX/target/generated -I../TouchGFX/target -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Utilities/JPEG -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/touchgfx/framework/include -I../TouchGFX/generated/fonts/include -I../TouchGFX/generated/gui_generated/include -I../TouchGFX/generated/images/include -I../TouchGFX/generated/texts/include -I../TouchGFX/generated/videos/include -I../TouchGFX/gui/include -I"C:/Users/user/GitHub/Display_TouchGFX_OWLAB/Display_Creation_TEST/Core/Inc/BSP/Font" -I"C:/Users/user/GitHub/Display_TouchGFX_OWLAB/Display_Creation_TEST/Core/Inc/BSP/STM32H7_BSP" -I"C:/Users/user/GitHub/Display_TouchGFX_OWLAB/Display_Creation_TEST/Core/Inc/User_App/Display" -I"C:/Users/user/GitHub/Display_TouchGFX_OWLAB/Display_Creation_TEST/Core/Inc/User_App/General" -I"C:/Users/user/GitHub/Display_TouchGFX_OWLAB/Display_Creation_TEST/Core/Inc/User_App/Memory" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-BSP-2f-STM32H7_BSP

clean-Core-2f-Src-2f-BSP-2f-STM32H7_BSP:
	-$(RM) ./Core/Src/BSP/STM32H7_BSP/stm32h743i_eval.cyclo ./Core/Src/BSP/STM32H7_BSP/stm32h743i_eval.d ./Core/Src/BSP/STM32H7_BSP/stm32h743i_eval.o ./Core/Src/BSP/STM32H7_BSP/stm32h743i_eval.su ./Core/Src/BSP/STM32H7_BSP/stm32h743i_eval_eeprom.cyclo ./Core/Src/BSP/STM32H7_BSP/stm32h743i_eval_eeprom.d ./Core/Src/BSP/STM32H7_BSP/stm32h743i_eval_eeprom.o ./Core/Src/BSP/STM32H7_BSP/stm32h743i_eval_eeprom.su ./Core/Src/BSP/STM32H7_BSP/stm32h743i_eval_lcd.cyclo ./Core/Src/BSP/STM32H7_BSP/stm32h743i_eval_lcd.d ./Core/Src/BSP/STM32H7_BSP/stm32h743i_eval_lcd.o ./Core/Src/BSP/STM32H7_BSP/stm32h743i_eval_lcd.su

.PHONY: clean-Core-2f-Src-2f-BSP-2f-STM32H7_BSP

