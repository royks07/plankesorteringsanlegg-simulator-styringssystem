################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include glui/subdir.mk
-include freeglut/subdir.mk
-include Testbed/Tests/subdir.mk
-include Testbed/Framework/subdir.mk
-include HelloWorld/subdir.mk
-include Build/cmake/glui/CMakeFiles/glui.dir/subdir.mk
-include Build/cmake/freeglut/CMakeFiles/freeglut_static.dir/subdir.mk
-include Build/cmake/Testbed/CMakeFiles/Testbed.dir/Tests/subdir.mk
-include Build/cmake/Testbed/CMakeFiles/Testbed.dir/Framework/subdir.mk
-include Build/cmake/HelloWorld/CMakeFiles/HelloWorld.dir/subdir.mk
-include Build/cmake/CMakeFiles/CompilerIdCXX/subdir.mk
-include Build/cmake/CMakeFiles/CompilerIdC/subdir.mk
-include Build/cmake/Box2D/CMakeFiles/Box2D.dir/Rope/subdir.mk
-include Build/cmake/Box2D/CMakeFiles/Box2D.dir/Dynamics/Joints/subdir.mk
-include Build/cmake/Box2D/CMakeFiles/Box2D.dir/Dynamics/Contacts/subdir.mk
-include Build/cmake/Box2D/CMakeFiles/Box2D.dir/Dynamics/subdir.mk
-include Build/cmake/Box2D/CMakeFiles/Box2D.dir/Common/subdir.mk
-include Build/cmake/Box2D/CMakeFiles/Box2D.dir/Collision/Shapes/subdir.mk
-include Build/cmake/Box2D/CMakeFiles/Box2D.dir/Collision/subdir.mk
-include Box2D/Rope/subdir.mk
-include Box2D/Dynamics/Joints/subdir.mk
-include Box2D/Dynamics/Contacts/subdir.mk
-include Box2D/Dynamics/subdir.mk
-include Box2D/Common/subdir.mk
-include Box2D/Collision/Shapes/subdir.mk
-include Box2D/Collision/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: PlankesorteringsAnlegg

# Tool invocations
PlankesorteringsAnlegg: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross G++ Linker'
	g++  -o "PlankesorteringsAnlegg" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) PlankesorteringsAnlegg
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
