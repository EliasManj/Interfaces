################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Button.c" \
"../Sources/Camera.c" \
"../Sources/RGB.c" \
"../Sources/buffer.c" \
"../Sources/helper_functions.c" \
"../Sources/main.c" \

C_SRCS += \
../Sources/Button.c \
../Sources/Camera.c \
../Sources/RGB.c \
../Sources/buffer.c \
../Sources/helper_functions.c \
../Sources/main.c \

OBJS += \
./Sources/Button.o \
./Sources/Camera.o \
./Sources/RGB.o \
./Sources/buffer.o \
./Sources/helper_functions.o \
./Sources/main.o \

C_DEPS += \
./Sources/Button.d \
./Sources/Camera.d \
./Sources/RGB.d \
./Sources/buffer.d \
./Sources/helper_functions.d \
./Sources/main.d \

OBJS_QUOTED += \
"./Sources/Button.o" \
"./Sources/Camera.o" \
"./Sources/RGB.o" \
"./Sources/buffer.o" \
"./Sources/helper_functions.o" \
"./Sources/main.o" \

C_DEPS_QUOTED += \
"./Sources/Button.d" \
"./Sources/Camera.d" \
"./Sources/RGB.d" \
"./Sources/buffer.d" \
"./Sources/helper_functions.d" \
"./Sources/main.d" \

OBJS_OS_FORMAT += \
./Sources/Button.o \
./Sources/Camera.o \
./Sources/RGB.o \
./Sources/buffer.o \
./Sources/helper_functions.o \
./Sources/main.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/Button.o: ../Sources/Button.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Button.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Button.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Camera.o: ../Sources/Camera.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Camera.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Camera.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/RGB.o: ../Sources/RGB.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/RGB.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/RGB.o"
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


