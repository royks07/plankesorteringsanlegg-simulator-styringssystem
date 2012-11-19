################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../HelloWorld/HelloWorld.cpp 

OBJS += \
./HelloWorld/HelloWorld.o 

CPP_DEPS += \
./HelloWorld/HelloWorld.d 


# Each subdirectory must supply rules for building sources it contributes
HelloWorld/%.o: ../HelloWorld/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


