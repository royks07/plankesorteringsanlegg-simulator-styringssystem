################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Build/cmake/CMakeFiles/CompilerIdC/CMakeCCompilerId.c 

OBJS += \
./Build/cmake/CMakeFiles/CompilerIdC/CMakeCCompilerId.o 

C_DEPS += \
./Build/cmake/CMakeFiles/CompilerIdC/CMakeCCompilerId.d 


# Each subdirectory must supply rules for building sources it contributes
Build/cmake/CMakeFiles/CompilerIdC/%.o: ../Build/cmake/CMakeFiles/CompilerIdC/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


