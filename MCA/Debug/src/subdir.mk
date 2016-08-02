################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ClientReader.cpp \
../src/MessengerClient.cpp \
../src/main.cpp 

OBJS += \
./src/ClientReader.o \
./src/MessengerClient.o \
./src/main.o 

CPP_DEPS += \
./src/ClientReader.d \
./src/MessengerClient.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/user/project4/SocketUtils/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


