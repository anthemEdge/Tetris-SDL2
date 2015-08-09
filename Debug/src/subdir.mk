################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Graphics.cpp \
../src/LTexture.cpp \
../src/LTimer.cpp \
../src/Playfield.cpp \
../src/Tetris.cpp \
../src/Tetromino.cpp 

OBJS += \
./src/Graphics.o \
./src/LTexture.o \
./src/LTimer.o \
./src/Playfield.o \
./src/Tetris.o \
./src/Tetromino.o 

CPP_DEPS += \
./src/Graphics.d \
./src/LTexture.d \
./src/LTimer.d \
./src/Playfield.d \
./src/Tetris.d \
./src/Tetromino.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/SDL2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


