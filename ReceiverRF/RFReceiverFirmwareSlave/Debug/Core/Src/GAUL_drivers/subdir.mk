################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/GAUL_drivers/I2C_Slave.c \
../Core/Src/GAUL_drivers/MLX90393.c \
../Core/Src/GAUL_drivers/Pulse_pin.c \
../Core/Src/GAUL_drivers/RFM22.c \
../Core/Src/GAUL_drivers/buzzer.c \
../Core/Src/GAUL_drivers/i2c_lcd.c \
../Core/Src/GAUL_drivers/timer.c 

OBJS += \
./Core/Src/GAUL_drivers/I2C_Slave.o \
./Core/Src/GAUL_drivers/MLX90393.o \
./Core/Src/GAUL_drivers/Pulse_pin.o \
./Core/Src/GAUL_drivers/RFM22.o \
./Core/Src/GAUL_drivers/buzzer.o \
./Core/Src/GAUL_drivers/i2c_lcd.o \
./Core/Src/GAUL_drivers/timer.o 

C_DEPS += \
./Core/Src/GAUL_drivers/I2C_Slave.d \
./Core/Src/GAUL_drivers/MLX90393.d \
./Core/Src/GAUL_drivers/Pulse_pin.d \
./Core/Src/GAUL_drivers/RFM22.d \
./Core/Src/GAUL_drivers/buzzer.d \
./Core/Src/GAUL_drivers/i2c_lcd.d \
./Core/Src/GAUL_drivers/timer.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/GAUL_drivers/%.o Core/Src/GAUL_drivers/%.su Core/Src/GAUL_drivers/%.cyclo: ../Core/Src/GAUL_drivers/%.c Core/Src/GAUL_drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-GAUL_drivers

clean-Core-2f-Src-2f-GAUL_drivers:
	-$(RM) ./Core/Src/GAUL_drivers/I2C_Slave.cyclo ./Core/Src/GAUL_drivers/I2C_Slave.d ./Core/Src/GAUL_drivers/I2C_Slave.o ./Core/Src/GAUL_drivers/I2C_Slave.su ./Core/Src/GAUL_drivers/MLX90393.cyclo ./Core/Src/GAUL_drivers/MLX90393.d ./Core/Src/GAUL_drivers/MLX90393.o ./Core/Src/GAUL_drivers/MLX90393.su ./Core/Src/GAUL_drivers/Pulse_pin.cyclo ./Core/Src/GAUL_drivers/Pulse_pin.d ./Core/Src/GAUL_drivers/Pulse_pin.o ./Core/Src/GAUL_drivers/Pulse_pin.su ./Core/Src/GAUL_drivers/RFM22.cyclo ./Core/Src/GAUL_drivers/RFM22.d ./Core/Src/GAUL_drivers/RFM22.o ./Core/Src/GAUL_drivers/RFM22.su ./Core/Src/GAUL_drivers/buzzer.cyclo ./Core/Src/GAUL_drivers/buzzer.d ./Core/Src/GAUL_drivers/buzzer.o ./Core/Src/GAUL_drivers/buzzer.su ./Core/Src/GAUL_drivers/i2c_lcd.cyclo ./Core/Src/GAUL_drivers/i2c_lcd.d ./Core/Src/GAUL_drivers/i2c_lcd.o ./Core/Src/GAUL_drivers/i2c_lcd.su ./Core/Src/GAUL_drivers/timer.cyclo ./Core/Src/GAUL_drivers/timer.d ./Core/Src/GAUL_drivers/timer.o ./Core/Src/GAUL_drivers/timer.su

.PHONY: clean-Core-2f-Src-2f-GAUL_drivers

