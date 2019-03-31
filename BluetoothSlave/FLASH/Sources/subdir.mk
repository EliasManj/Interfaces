################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/RGB.c" \
"../Sources/buffer.c" \
"../Sources/cmd_parser.c" \
"../Sources/main.c" \
"../Sources/sa_mtb.c" \

C_SRCS += \
../Sources/RGB.c \
../Sources/buffer.c \
../Sources/cmd_parser.c \
../Sources/main.c \
../Sources/sa_mtb.c \

OBJS += \
./Sources/RGB.o \
./Sources/buffer.o \
./Sources/cmd_parser.o \
./Sources/main.o \
./Sources/sa_mtb.o \

C_DEPS += \
./Sources/RGB.d \
./Sources/buffer.d \
./Sources/cmd_parser.d \
./Sources/main.d \
./Sources/sa_mtb.d \

OBJS_QUOTED += \
"./Sources/RGB.o" \
"./Sources/buffer.o" \
"./Sources/cmd_parser.o" \
"./Sources/main.o" \
"./Sources/sa_mtb.o" \

C_DEPS_QUOTED += \
"./Sources/RGB.d" \
"./Sources/buffer.d" \
"./Sources/cmd_parser.d" \
"./Sources/main.d" \
"./Sources/sa_mtb.d" \

OBJS_OS_FORMAT += \
./Sources/RGB.o \
./Sources/buffer.o \
./Sources/cmd_parser.o \
./Sources/main.o \
./Sources/sa_mtb.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/RGB.o: ../Sources/RGB.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/RGB.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/RGB.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/buffer.o: ../Sources/buffer.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/buffer.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/buffer.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/cmd_parser.o: ../Sources/cmd_parser.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/cmd_parser.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/cmd_parser.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/sa_mtb.o: ../Sources/sa_mtb.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/sa_mtb.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/sa_mtb.o"
	@echo 'Finished building: $<'
	@echo ' '


