################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/RGB.c" \
"../Sources/Wifi.c" \
"../Sources/Wifi_Uart_Interface.c" \
"../Sources/buffer.c" \
"../Sources/helper_functions.c" \
"../Sources/main.c" \

C_SRCS += \
../Sources/RGB.c \
../Sources/Wifi.c \
../Sources/Wifi_Uart_Interface.c \
../Sources/buffer.c \
../Sources/helper_functions.c \
../Sources/main.c \

OBJS += \
./Sources/RGB.o \
./Sources/Wifi.o \
./Sources/Wifi_Uart_Interface.o \
./Sources/buffer.o \
./Sources/helper_functions.o \
./Sources/main.o \

C_DEPS += \
./Sources/RGB.d \
./Sources/Wifi.d \
./Sources/Wifi_Uart_Interface.d \
./Sources/buffer.d \
./Sources/helper_functions.d \
./Sources/main.d \

OBJS_QUOTED += \
"./Sources/RGB.o" \
"./Sources/Wifi.o" \
"./Sources/Wifi_Uart_Interface.o" \
"./Sources/buffer.o" \
"./Sources/helper_functions.o" \
"./Sources/main.o" \

C_DEPS_QUOTED += \
"./Sources/RGB.d" \
"./Sources/Wifi.d" \
"./Sources/Wifi_Uart_Interface.d" \
"./Sources/buffer.d" \
"./Sources/helper_functions.d" \
"./Sources/main.d" \

OBJS_OS_FORMAT += \
./Sources/RGB.o \
./Sources/Wifi.o \
./Sources/Wifi_Uart_Interface.o \
./Sources/buffer.o \
./Sources/helper_functions.o \
./Sources/main.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/RGB.o: ../Sources/RGB.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/RGB.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/RGB.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Wifi.o: ../Sources/Wifi.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Wifi.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Wifi.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Wifi_Uart_Interface.o: ../Sources/Wifi_Uart_Interface.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Wifi_Uart_Interface.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Wifi_Uart_Interface.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/buffer.o: ../Sources/buffer.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/buffer.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/buffer.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/helper_functions.o: ../Sources/helper_functions.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/helper_functions.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/helper_functions.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '


