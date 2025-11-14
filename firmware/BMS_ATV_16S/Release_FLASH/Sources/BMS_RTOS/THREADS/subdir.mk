################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/BMS_RTOS/THREADS/RTOS_AFE_Initialisation_entry.c \
../Sources/BMS_RTOS/THREADS/RTOS_BMS_StateProccessingThread_entry.c \
../Sources/BMS_RTOS/THREADS/RTOS_DataProcessingThread_entry.c \
../Sources/BMS_RTOS/THREADS/RTOS_QueueReceiveThread_entry.c \
../Sources/BMS_RTOS/THREADS/RTOS_SoCEstimationThread_entry.c \
../Sources/BMS_RTOS/THREADS/RTOS_TimerCallback_entry.c \
../Sources/BMS_RTOS/THREADS/RTOS_WDGThread_entry.c 

OBJS += \
./Sources/BMS_RTOS/THREADS/RTOS_AFE_Initialisation_entry.o \
./Sources/BMS_RTOS/THREADS/RTOS_BMS_StateProccessingThread_entry.o \
./Sources/BMS_RTOS/THREADS/RTOS_DataProcessingThread_entry.o \
./Sources/BMS_RTOS/THREADS/RTOS_QueueReceiveThread_entry.o \
./Sources/BMS_RTOS/THREADS/RTOS_SoCEstimationThread_entry.o \
./Sources/BMS_RTOS/THREADS/RTOS_TimerCallback_entry.o \
./Sources/BMS_RTOS/THREADS/RTOS_WDGThread_entry.o 

C_DEPS += \
./Sources/BMS_RTOS/THREADS/RTOS_AFE_Initialisation_entry.d \
./Sources/BMS_RTOS/THREADS/RTOS_BMS_StateProccessingThread_entry.d \
./Sources/BMS_RTOS/THREADS/RTOS_DataProcessingThread_entry.d \
./Sources/BMS_RTOS/THREADS/RTOS_QueueReceiveThread_entry.d \
./Sources/BMS_RTOS/THREADS/RTOS_SoCEstimationThread_entry.d \
./Sources/BMS_RTOS/THREADS/RTOS_TimerCallback_entry.d \
./Sources/BMS_RTOS/THREADS/RTOS_WDGThread_entry.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/BMS_RTOS/THREADS/%.o: ../Sources/BMS_RTOS/THREADS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/BMS_RTOS/THREADS/RTOS_AFE_Initialisation_entry.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


