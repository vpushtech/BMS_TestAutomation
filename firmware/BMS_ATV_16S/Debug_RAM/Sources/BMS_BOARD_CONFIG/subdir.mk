################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/BMS_BOARD_CONFIG/Board_Config.c 

OBJS += \
./Sources/BMS_BOARD_CONFIG/Board_Config.o 

C_DEPS += \
./Sources/BMS_BOARD_CONFIG/Board_Config.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/BMS_BOARD_CONFIG/%.o: ../Sources/BMS_BOARD_CONFIG/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/BMS_BOARD_CONFIG/Board_Config.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


