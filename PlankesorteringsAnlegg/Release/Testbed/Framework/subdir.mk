################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Testbed/Framework/Main.cpp \
../Testbed/Framework/Render.cpp \
../Testbed/Framework/Test.cpp \
../Testbed/Framework/b2dJson.cpp \
../Testbed/Framework/jsoncpp.cpp 

OBJS += \
./Testbed/Framework/Main.o \
./Testbed/Framework/Render.o \
./Testbed/Framework/Test.o \
./Testbed/Framework/b2dJson.o \
./Testbed/Framework/jsoncpp.o 

CPP_DEPS += \
./Testbed/Framework/Main.d \
./Testbed/Framework/Render.d \
./Testbed/Framework/Test.d \
./Testbed/Framework/b2dJson.d \
./Testbed/Framework/jsoncpp.d 


# Each subdirectory must supply rules for building sources it contributes
Testbed/Framework/%.o: ../Testbed/Framework/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


