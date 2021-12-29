#include <fstream>
#include <iostream>
#include "binary_alu_cmd.h"

namespace vm_command {

	void BinaryALUCommand::prepend(std::ofstream &out) const {
		write_asm(out, "@SP");
		write_asm(out, "AM=M-1");
		write_asm(out, "D=M");
		write_asm(out, "@SP");
		write_asm(out, "A=M-1");
	}

}
