################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Buffer.c" \
"../Sources/GPS.c" \
"../Sources/main.c" \

C_SRCS += \
../Sources/Buffer.c \
../Sources/GPS.c \
../Sources/main.c \

OBJS += \
./Sources/Buffer.o \
./Sources/GPS.o \
./Sources/main.o \

C_DEPS += \
./Sources/Buffer.d \
./Sources/GPS.d \
./Sources/main.d \

OBJS_QUOTED += \
"./Sources/Buffer.o" \
"./Sources/GPS.o" \
"./Sources/main.o" \

C_DEPS_QUOTED += \
"./Sources/Buffer.d" \
"./Sources/GPS.d" \
"./Sources/main.d" \

OBJS_OS_FORMAT += \
./Sources/Buffer.o \
./Sources/GPS.o \
./Sources/main.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/Buffer.o: ../Sources/Buffer.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Buffer.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Buffer.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/GPS.o: ../Sources/GPS.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/GPS.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/GPS.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '

