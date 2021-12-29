#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <utility>
#include "command.h"

using CPUResult = std::pair<std::string, int>;

class Utils {
	public:

	static CPUResult run_cpu_emulator(const std::string &output_asm_file);
	static CPUResult run_cpu_emulator(const vm_command::Command &command,
									  const std::string &output_asm_file);

};

#endif
