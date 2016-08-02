################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Brocker.cpp \
../src/ChatRoom.cpp \
../src/Dispatcher.cpp \
../src/MessengerServer.cpp \
../src/Users.cpp \
../src/main.cpp 

OBJS += \
./src/Brocker.o \
./src/ChatRoom.o \
./src/Dispatcher.o \
./src/MessengerServer.o \
./src/Users.o \
./src/main.o 

CPP_DEPS += \
./src/Brocker.d \
./src/ChatRoom.d \
./src/Dispatcher.d \
./src/MessengerServer.d \
./src/Users.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/user/project4/SocketUtils/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


