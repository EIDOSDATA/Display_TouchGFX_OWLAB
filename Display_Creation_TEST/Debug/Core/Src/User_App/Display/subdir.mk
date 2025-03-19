################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/User_App/Display/user_disp.c \
../Core/Src/User_App/Display/user_disp_touch.c 

C_DEPS += \
./Core/Src/User_App/Display/user_disp.d \
./Core/Src/User_App/Display/user_disp_touch.d 

OBJS += \
./Core/Src/User_App/Display/user_disp.o \
./Core/Src/User_App/Display/user_disp_touch.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/User_App/Display/%.o Core/Src/User_App/Display/%.su Core/Src/User_App/Display/%.cyclo: ../Core/Src/User_App/Display/%.c Core/Src/User_App/Display/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../TouchGFX/App -I../TouchGFX/target/generated -I../TouchGFX/target -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Utilities/JPEG -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/touchgfx/framework/include -I../TouchGFX/generated/fonts/include -I../TouchGFX/generated/gui_generated/include -I../TouchGFX/generated/images/include -I../TouchGFX/generated/texts/include -I../TouchGFX/generated/videos/include -I../TouchGFX/gui/include -I"C:/Users/user/GitHub/OW_DISPLAY_BOARD/Display_Creation_TEST/Core/Inc/BSP/Font" -I"C:/Users/user/GitHub/OW_DISPLAY_BOARD/Display_Creation_TEST/Core/Inc/BSP/STM32H7_BSP" -I"C:/Users/user/GitHub/OW_DISPLAY_BOARD/Display_Creation_TEST/Core/Inc/User_App/Display" -I"C:/Users/user/GitHub/OW_DISPLAY_BOARD/Display_Creation_TEST/Core/Inc/User_App/General" -I"C:/Users/user/GitHub/OW_DISPLAY_BOARD/Display_Creation_TEST/Core/Inc/User_App/Memory" -I"C:/Users/user/GitHub/OW_DISPLAY_BOARD/Display_Creation_TEST/Core/Inc/driver" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-User_App-2f-Display

clean-Core-2f-Src-2f-User_App-2f-Display:
	-$(RM) ./Core/Src/User_App/Display/user_disp.cyclo ./Core/Src/User_App/Display/user_disp.d ./Core/Src/User_App/Display/user_disp.o ./Core/Src/User_App/Display/user_disp.su ./Core/Src/User_App/Display/user_disp_touch.cyclo ./Core/Src/User_App/Display/user_disp_touch.d ./Core/Src/User_App/Display/user_disp_touch.o ./Core/Src/User_App/Display/user_disp_touch.su

.PHONY: clean-Core-2f-Src-2f-User_App-2f-Display

