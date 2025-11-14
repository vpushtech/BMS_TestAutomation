################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/BMS_RTOS/VPUSH_RTOS.c 

OBJS += \
./Sources/BMS_RTOS/VPUSH_RTOS.o 

C_DEPS += \
./Sources/BMS_RTOS/VPUSH_RTOS.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/BMS_RTOS/%.o: ../Sources/BMS_RTOS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/BMS_RTOS/VPUSH_RTOS.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


