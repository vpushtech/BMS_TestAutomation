################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/BMS_DRIVERS/AFE_bq76952.c 

OBJS += \
./Sources/BMS_DRIVERS/AFE_bq76952.o 

C_DEPS += \
./Sources/BMS_DRIVERS/AFE_bq76952.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/BMS_DRIVERS/%.o: ../Sources/BMS_DRIVERS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/BMS_DRIVERS/AFE_bq76952.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


