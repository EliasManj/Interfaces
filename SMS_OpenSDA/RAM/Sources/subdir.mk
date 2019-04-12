################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Message_Parser.c" \
"../Sources/buffer.c" \
"../Sources/main.c" \

C_SRCS += \
../Sources/Message_Parser.c \
../Sources/buffer.c \
../Sources/main.c \

OBJS += \
./Sources/Message_Parser.o \
./Sources/buffer.o \
./Sources/main.o \

C_DEPS += \
./Sources/Message_Parser.d \
./Sources/buffer.d \
./Sources/main.d \

OBJS_QUOTED += \
"./Sources/Message_Parser.o" \
"./Sources/buffer.o" \
"./Sources/main.o" \

C_DEPS_QUOTED += \
"./Sources/Message_Parser.d" \
"./Sources/buffer.d" \
"./Sources/main.d" \

OBJS_OS_FORMAT += \
./Sources/Message_Parser.o \
./Sources/buffer.o \
./Sources/main.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/Message_Parser.o: ../Sources/Message_Parser.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Message_Parser.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Message_Parser.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/buffer.o: ../Sources/buffer.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/buffer.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/buffer.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '


