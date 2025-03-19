################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/BSP/Font/font12.c \
../Core/Src/BSP/Font/font16.c \
../Core/Src/BSP/Font/font20.c \
../Core/Src/BSP/Font/font24.c \
../Core/Src/BSP/Font/font8.c 

C_DEPS += \
./Core/Src/BSP/Font/font12.d \
./Core/Src/BSP/Font/font16.d \
./Core/Src/BSP/Font/font20.d \
./Core/Src/BSP/Font/font24.d \
./Core/Src/BSP/Font/font8.d 

OBJS += \
./Core/Src/BSP/Font/font12.o \
./Core/Src/BSP/Font/font16.o \
./Core/Src/BSP/Font/font20.o \
./Core/Src/BSP/Font/font24.o \
./Core/Src/BSP/Font/font8.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/BSP/Font/%.o Core/Src/BSP/Font/%.su Core/Src/BSP/Font/%.cyclo: ../Core/Src/BSP/Font/%.c Core/Src/BSP/Font/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../TouchGFX/App -I../TouchGFX/target/generated -I../TouchGFX/target -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Utilities/JPEG -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/touchgfx/framework/include -I../TouchGFX/generated/fonts/include -I../TouchGFX/generated/gui_generated/include -I../TouchGFX/generated/images/include -I../TouchGFX/generated/texts/include -I../TouchGFX/generated/videos/include -I../TouchGFX/gui/include -I"C:/Users/user/GitHub/Display_TouchGFX_OWLAB/Display_Creation_TEST/Core/Inc/BSP/Font" -I"C:/Users/user/GitHub/Display_TouchGFX_OWLAB/Display_Creation_TEST/Core/Inc/BSP/STM32H7_BSP" -I"C:/Users/user/GitHub/Display_TouchGFX_OWLAB/Display_Creation_TEST/Core/Inc/User_App/Display" -I"C:/Users/user/GitHub/Display_TouchGFX_OWLAB/Display_Creation_TEST/Core/Inc/User_App/General" -I"C:/Users/user/GitHub/Display_TouchGFX_OWLAB/Display_Creation_TEST/Core/Inc/User_App/Memory" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-BSP-2f-Font

clean-Core-2f-Src-2f-BSP-2f-Font:
	-$(RM) ./Core/Src/BSP/Font/font12.cyclo ./Core/Src/BSP/Font/font12.d ./Core/Src/BSP/Font/font12.o ./Core/Src/BSP/Font/font12.su ./Core/Src/BSP/Font/font16.cyclo ./Core/Src/BSP/Font/font16.d ./Core/Src/BSP/Font/font16.o ./Core/Src/BSP/Font/font16.su ./Core/Src/BSP/Font/font20.cyclo ./Core/Src/BSP/Font/font20.d ./Core/Src/BSP/Font/font20.o ./Core/Src/BSP/Font/font20.su ./Core/Src/BSP/Font/font24.cyclo ./Core/Src/BSP/Font/font24.d ./Core/Src/BSP/Font/font24.o ./Core/Src/BSP/Font/font24.su ./Core/Src/BSP/Font/font8.cyclo ./Core/Src/BSP/Font/font8.d ./Core/Src/BSP/Font/font8.o ./Core/Src/BSP/Font/font8.su

.PHONY: clean-Core-2f-Src-2f-BSP-2f-Font

