#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <utility>
#include "command.h"

using CPUResult = std::pair<std::string, int>;

/* A helper class that defines utility methods */
class Utils {
	public:

		/* runs Nand2tetris Version 2.6 CPU Emulator using the provided assembly file.
			
			The associated .tst and .cmp files are expected to be found in the same
			directory as the input file.

			It returns a pair of the emulator's output and its return status.
		*/
		static CPUResult run_cpu_emulator(const std::string &output_asm_file);

		/*
			similar to the above method, it runs the emulator; however, the content
			of the input file is the result of executing vm_command::Command::to_asm method.
		*/
		static CPUResult run_cpu_emulator(const vm_command::Command &command,
										  const std::string &output_asm_file);

};

#endif
