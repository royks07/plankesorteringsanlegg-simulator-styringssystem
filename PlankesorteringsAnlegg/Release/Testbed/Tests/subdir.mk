################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Testbed/Tests/TestEntries.cpp 

OBJS += \
./Testbed/Tests/TestEntries.o 

CPP_DEPS += \
./Testbed/Tests/TestEntries.d 


# Each subdirectory must supply rules for building sources it contributes
Testbed/Tests/%.o: ../Testbed/Tests/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


