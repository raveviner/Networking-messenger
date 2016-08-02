################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/FileInterface.cpp \
../src/MThread.cpp \
../src/MultipleTCPSocketListener.cpp \
../src/Socket.cpp \
../src/TCPSocket.cpp \
../src/UDPSocket.cpp 

OBJS += \
./src/FileInterface.o \
./src/MThread.o \
./src/MultipleTCPSocketListener.o \
./src/Socket.o \
./src/TCPSocket.o \
./src/UDPSocket.o 

CPP_DEPS += \
./src/FileInterface.d \
./src/MThread.d \
./src/MultipleTCPSocketListener.d \
./src/Socket.d \
./src/TCPSocket.d \
./src/UDPSocket.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


