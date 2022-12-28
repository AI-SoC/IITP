################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Configurations/Debug/sync_on_halt.c 

OBJS += \
./Configurations/Debug/sync_on_halt.o 

COMPILED_SRCS += \
./Configurations/Debug/sync_on_halt.src 

C_DEPS += \
./Configurations/Debug/sync_on_halt.d 


# Each subdirectory must supply rules for building sources it contributes
Configurations/Debug/%.src: ../Configurations/Debug/%.c Configurations/Debug/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc27xd "-fC:/JSKWON/AURIX/2022_IEMEK_DISTRIBUTED_DSP/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

Configurations/Debug/%.o: ./Configurations/Debug/%.src Configurations/Debug/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


