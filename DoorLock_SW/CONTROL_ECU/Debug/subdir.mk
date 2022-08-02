################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LCD.c \
../buzzer.c \
../control.c \
../control_main.c \
../external_eeprom.c \
../gpio.c \
../motor.c \
../timer.c \
../twi.c \
../uart.c 

OBJS += \
./LCD.o \
./buzzer.o \
./control.o \
./control_main.o \
./external_eeprom.o \
./gpio.o \
./motor.o \
./timer.o \
./twi.o \
./uart.o 

C_DEPS += \
./LCD.d \
./buzzer.d \
./control.d \
./control_main.d \
./external_eeprom.d \
./gpio.d \
./motor.d \
./timer.d \
./twi.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


