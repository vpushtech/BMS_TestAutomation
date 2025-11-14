################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_adc.c \
../Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_can.c \
../Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_flash.c \
../Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_gpio.c \
../Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_i2c.c \
../Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_nvic.c \
../Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_timer.c \
../Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_wdg.c 

OBJS += \
./Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_adc.o \
./Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_can.o \
./Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_flash.o \
./Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_gpio.o \
./Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_i2c.o \
./Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_nvic.o \
./Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_timer.o \
./Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_wdg.o 

C_DEPS += \
./Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_adc.d \
./Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_can.d \
./Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_flash.d \
./Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_gpio.d \
./Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_i2c.d \
./Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_nvic.d \
./Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_timer.d \
./Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_wdg.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/BMS_DRIVERS/PERIPH_DRIVERS/%.o: ../Sources/BMS_DRIVERS/PERIPH_DRIVERS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/BMS_DRIVERS/PERIPH_DRIVERS/drv_adc.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


