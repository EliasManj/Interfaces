################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/GPS.c" \
"../Sources/main.c" \

C_SRCS += \
../Sources/GPS.c \
../Sources/main.c \

OBJS += \
./Sources/GPS.o \
./Sources/main.o \

C_DEPS += \
./Sources/GPS.d \
./Sources/main.d \

OBJS_QUOTED += \
"./Sources/GPS.o" \
"./Sources/main.o" \

C_DEPS_QUOTED += \
"./Sources/GPS.d" \
"./Sources/main.d" \

OBJS_OS_FORMAT += \
./Sources/GPS.o \
./Sources/main.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/GPS.o: ../Sources/GPS.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/GPS.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/GPS.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '


