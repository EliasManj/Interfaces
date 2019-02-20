################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/codigoCincel.c" \
"../Sources/main.c" \

C_SRCS += \
../Sources/codigoCincel.c \
../Sources/main.c \

OBJS += \
./Sources/codigoCincel.o \
./Sources/main.o \

C_DEPS += \
./Sources/codigoCincel.d \
./Sources/main.d \

OBJS_QUOTED += \
"./Sources/codigoCincel.o" \
"./Sources/main.o" \

C_DEPS_QUOTED += \
"./Sources/codigoCincel.d" \
"./Sources/main.d" \

OBJS_OS_FORMAT += \
./Sources/codigoCincel.o \
./Sources/main.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/codigoCincel.o: ../Sources/codigoCincel.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/codigoCincel.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/codigoCincel.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '


