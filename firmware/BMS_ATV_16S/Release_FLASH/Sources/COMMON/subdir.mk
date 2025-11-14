################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/COMMON/common.c 

OBJS += \
./Sources/COMMON/common.o 

C_DEPS += \
./Sources/COMMON/common.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/COMMON/%.o: ../Sources/COMMON/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/COMMON/common.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


