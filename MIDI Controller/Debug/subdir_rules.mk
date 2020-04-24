################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccs900/ccs/tools/compiler/msp430-gcc-7.3.2.154_win32/bin/msp430-elf-gcc-7.3.2.exe" -c -mmcu=msp430fr5969 -mhwmult=f5series -I"C:/ti/ccs900/ccs/ccs_base/msp430/include_gcc" -I"C:/Users/julia/workspace_v9/MIDI Controller" -I"C:/ti/ccs900/ccs/tools/compiler/msp430-gcc-7.3.2.154_win32/msp430-elf/include" -Og -g -gdwarf-3 -gstrict-dwarf -Wall -mlarge -mcode-region=none -mdata-region=lower -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


