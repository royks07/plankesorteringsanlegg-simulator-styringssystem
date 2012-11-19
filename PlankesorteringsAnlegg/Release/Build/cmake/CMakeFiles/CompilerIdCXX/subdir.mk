################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Build/cmake/CMakeFiles/CompilerIdCXX/CMakeCXXCompilerId.cpp 

OBJS += \
./Build/cmake/CMakeFiles/CompilerIdCXX/CMakeCXXCompilerId.o 

CPP_DEPS += \
./Build/cmake/CMakeFiles/CompilerIdCXX/CMakeCXXCompilerId.d 


# Each subdirectory must supply rules for building sources it contributes
Build/cmake/CMakeFiles/CompilerIdCXX/%.o: ../Build/cmake/CMakeFiles/CompilerIdCXX/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


