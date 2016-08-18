################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
myIncludes/richGPS.obj: ../myIncludes/richGPS.cpp $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.13/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.13/include" --include_path="C:/TM4C123G_LaunchPad_Workshop/lab12/project/myIncludes" --include_path="C:/TI/TivaWare_C_Series-1.1" -g --define=DEBUG --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 --diag_wrap=off --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="myIncludes/richGPS.pp" --obj_directory="myIncludes" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


