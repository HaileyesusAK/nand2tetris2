#include <fstream>
#include <iostream>
#include "relational_cmd.h"

namespace vm_command {

	void RelCommand::prepend(std::ofstream &out) const {
		write_asm(out, "@SP");
		write_asm(out, "AM=M-1");
		write_asm(out, "D=M");
		write_asm(out, "@SP");
		write_asm(out, "A=M-1");
		write_asm(out, "D=M-D");
		write_asm(out, "M=-1");
	}

	void RelCommand::append(std::ofstream &out) const {
		write_asm(out, "@SP");
		write_asm(out, "A=M-1");
		write_asm(out, "M=0");
	}

}
