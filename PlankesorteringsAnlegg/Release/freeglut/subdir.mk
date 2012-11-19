################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freeglut/freeglut_callbacks.c \
../freeglut/freeglut_cursor.c \
../freeglut/freeglut_display.c \
../freeglut/freeglut_ext.c \
../freeglut/freeglut_font.c \
../freeglut/freeglut_font_data.c \
../freeglut/freeglut_gamemode.c \
../freeglut/freeglut_geometry.c \
../freeglut/freeglut_glutfont_definitions.c \
../freeglut/freeglut_init.c \
../freeglut/freeglut_input_devices.c \
../freeglut/freeglut_joystick.c \
../freeglut/freeglut_main.c \
../freeglut/freeglut_menu.c \
../freeglut/freeglut_misc.c \
../freeglut/freeglut_overlay.c \
../freeglut/freeglut_spaceball.c \
../freeglut/freeglut_state.c \
../freeglut/freeglut_stroke_mono_roman.c \
../freeglut/freeglut_stroke_roman.c \
../freeglut/freeglut_structure.c \
../freeglut/freeglut_teapot.c \
../freeglut/freeglut_videoresize.c \
../freeglut/freeglut_window.c 

OBJS += \
./freeglut/freeglut_callbacks.o \
./freeglut/freeglut_cursor.o \
./freeglut/freeglut_display.o \
./freeglut/freeglut_ext.o \
./freeglut/freeglut_font.o \
./freeglut/freeglut_font_data.o \
./freeglut/freeglut_gamemode.o \
./freeglut/freeglut_geometry.o \
./freeglut/freeglut_glutfont_definitions.o \
./freeglut/freeglut_init.o \
./freeglut/freeglut_input_devices.o \
./freeglut/freeglut_joystick.o \
./freeglut/freeglut_main.o \
./freeglut/freeglut_menu.o \
./freeglut/freeglut_misc.o \
./freeglut/freeglut_overlay.o \
./freeglut/freeglut_spaceball.o \
./freeglut/freeglut_state.o \
./freeglut/freeglut_stroke_mono_roman.o \
./freeglut/freeglut_stroke_roman.o \
./freeglut/freeglut_structure.o \
./freeglut/freeglut_teapot.o \
./freeglut/freeglut_videoresize.o \
./freeglut/freeglut_window.o 

C_DEPS += \
./freeglut/freeglut_callbacks.d \
./freeglut/freeglut_cursor.d \
./freeglut/freeglut_display.d \
./freeglut/freeglut_ext.d \
./freeglut/freeglut_font.d \
./freeglut/freeglut_font_data.d \
./freeglut/freeglut_gamemode.d \
./freeglut/freeglut_geometry.d \
./freeglut/freeglut_glutfont_definitions.d \
./freeglut/freeglut_init.d \
./freeglut/freeglut_input_devices.d \
./freeglut/freeglut_joystick.d \
./freeglut/freeglut_main.d \
./freeglut/freeglut_menu.d \
./freeglut/freeglut_misc.d \
./freeglut/freeglut_overlay.d \
./freeglut/freeglut_spaceball.d \
./freeglut/freeglut_state.d \
./freeglut/freeglut_stroke_mono_roman.d \
./freeglut/freeglut_stroke_roman.d \
./freeglut/freeglut_structure.d \
./freeglut/freeglut_teapot.d \
./freeglut/freeglut_videoresize.d \
./freeglut/freeglut_window.d 


# Each subdirectory must supply rules for building sources it contributes
freeglut/%.o: ../freeglut/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


