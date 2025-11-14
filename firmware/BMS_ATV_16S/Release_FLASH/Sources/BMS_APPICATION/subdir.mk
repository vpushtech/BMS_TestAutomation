################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/BMS_APPICATION/APP_Callback.c \
../Sources/BMS_APPICATION/Application_BMS_HSM.c \
../Sources/BMS_APPICATION/Application_BMS_StateProccessing.c \
../Sources/BMS_APPICATION/Application_SafetyCheck.c 

OBJS += \
./Sources/BMS_APPICATION/APP_Callback.o \
./Sources/BMS_APPICATION/Application_BMS_HSM.o \
./Sources/BMS_APPICATION/Application_BMS_StateProccessing.o \
./Sources/BMS_APPICATION/Application_SafetyCheck.o 

C_DEPS += \
./Sources/BMS_APPICATION/APP_Callback.d \
./Sources/BMS_APPICATION/Application_BMS_HSM.d \
./Sources/BMS_APPICATION/Application_BMS_StateProccessing.d \
./Sources/BMS_APPICATION/Application_SafetyCheck.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/BMS_APPICATION/%.o: ../Sources/BMS_APPICATION/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/BMS_APPICATION/APP_Callback.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


