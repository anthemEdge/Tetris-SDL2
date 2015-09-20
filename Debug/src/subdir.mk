################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Graphics.cpp \
../src/LTexture.cpp \
../src/LTimer.cpp \
../src/Playfield.cpp \
../src/PlayfieldNode.cpp \
../src/Tetris.cpp \
../src/TetrisAI.cpp \
../src/Tetromino.cpp 

OBJS += \
./src/Graphics.o \
./src/LTexture.o \
./src/LTimer.o \
./src/Playfield.o \
./src/PlayfieldNode.o \
./src/Tetris.o \
./src/TetrisAI.o \
./src/Tetromino.o 

CPP_DEPS += \
./src/Graphics.d \
./src/LTexture.d \
./src/LTimer.d \
./src/Playfield.d \
./src/PlayfieldNode.d \
./src/Tetris.d \
./src/TetrisAI.d \
./src/Tetromino.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/SDL2 -I/usr/include/boost -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


