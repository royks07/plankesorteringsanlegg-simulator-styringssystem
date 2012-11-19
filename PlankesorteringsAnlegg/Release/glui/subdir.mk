################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../glui/algebra3.cpp \
../glui/arcball.cpp \
../glui/glui.cpp \
../glui/glui_add_controls.cpp \
../glui/glui_bitmap_img_data.cpp \
../glui/glui_bitmaps.cpp \
../glui/glui_button.cpp \
../glui/glui_checkbox.cpp \
../glui/glui_column.cpp \
../glui/glui_commandline.cpp \
../glui/glui_control.cpp \
../glui/glui_edittext.cpp \
../glui/glui_filebrowser.cpp \
../glui/glui_list.cpp \
../glui/glui_listbox.cpp \
../glui/glui_mouse_iaction.cpp \
../glui/glui_node.cpp \
../glui/glui_panel.cpp \
../glui/glui_radio.cpp \
../glui/glui_rollout.cpp \
../glui/glui_rotation.cpp \
../glui/glui_scrollbar.cpp \
../glui/glui_separator.cpp \
../glui/glui_spinner.cpp \
../glui/glui_statictext.cpp \
../glui/glui_string.cpp \
../glui/glui_textbox.cpp \
../glui/glui_translation.cpp \
../glui/glui_tree.cpp \
../glui/glui_treepanel.cpp \
../glui/glui_window.cpp \
../glui/quaternion.cpp 

OBJS += \
./glui/algebra3.o \
./glui/arcball.o \
./glui/glui.o \
./glui/glui_add_controls.o \
./glui/glui_bitmap_img_data.o \
./glui/glui_bitmaps.o \
./glui/glui_button.o \
./glui/glui_checkbox.o \
./glui/glui_column.o \
./glui/glui_commandline.o \
./glui/glui_control.o \
./glui/glui_edittext.o \
./glui/glui_filebrowser.o \
./glui/glui_list.o \
./glui/glui_listbox.o \
./glui/glui_mouse_iaction.o \
./glui/glui_node.o \
./glui/glui_panel.o \
./glui/glui_radio.o \
./glui/glui_rollout.o \
./glui/glui_rotation.o \
./glui/glui_scrollbar.o \
./glui/glui_separator.o \
./glui/glui_spinner.o \
./glui/glui_statictext.o \
./glui/glui_string.o \
./glui/glui_textbox.o \
./glui/glui_translation.o \
./glui/glui_tree.o \
./glui/glui_treepanel.o \
./glui/glui_window.o \
./glui/quaternion.o 

CPP_DEPS += \
./glui/algebra3.d \
./glui/arcball.d \
./glui/glui.d \
./glui/glui_add_controls.d \
./glui/glui_bitmap_img_data.d \
./glui/glui_bitmaps.d \
./glui/glui_button.d \
./glui/glui_checkbox.d \
./glui/glui_column.d \
./glui/glui_commandline.d \
./glui/glui_control.d \
./glui/glui_edittext.d \
./glui/glui_filebrowser.d \
./glui/glui_list.d \
./glui/glui_listbox.d \
./glui/glui_mouse_iaction.d \
./glui/glui_node.d \
./glui/glui_panel.d \
./glui/glui_radio.d \
./glui/glui_rollout.d \
./glui/glui_rotation.d \
./glui/glui_scrollbar.d \
./glui/glui_separator.d \
./glui/glui_spinner.d \
./glui/glui_statictext.d \
./glui/glui_string.d \
./glui/glui_textbox.d \
./glui/glui_translation.d \
./glui/glui_tree.d \
./glui/glui_treepanel.d \
./glui/glui_window.d \
./glui/quaternion.d 


# Each subdirectory must supply rules for building sources it contributes
glui/%.o: ../glui/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


