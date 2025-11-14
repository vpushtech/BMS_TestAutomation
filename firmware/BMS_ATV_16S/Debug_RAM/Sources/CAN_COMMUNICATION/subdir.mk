################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/CAN_COMMUNICATION/CAN_Communication.c 

OBJS += \
./Sources/CAN_COMMUNICATION/CAN_Communication.o 

C_DEPS += \
./Sources/CAN_COMMUNICATION/CAN_Communication.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/CAN_COMMUNICATION/%.o: ../Sources/CAN_COMMUNICATION/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/CAN_COMMUNICATION/CAN_Communication.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


