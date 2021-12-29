#include <fstream>
#include <iostream>
#include <string>
#include "push_static.h"

namespace vm_command {

	PushStatic::PushStatic(const std::string& file_name, uint16_t idx) {
		this->file_name = file_name;
		this->idx = idx;
	}

	void PushStatic::to_asm(std::ofstream &out) const {
        write_asm(out, "// push static " + std::to_string(idx), "");
        write_asm(out, "@" + file_name + "." + std::to_string(idx));
        write_asm(out, "D=M");
		write_asm(out, "@SP");
		write_asm(out, "A=M");
		write_asm(out, "M=D");
		write_asm(out, "@SP");
		write_asm(out, "M=M+1");
	}

}
