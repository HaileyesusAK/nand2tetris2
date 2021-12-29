#include <fstream>
#include <iostream>
#include <string>
#include "pop_static.h"

namespace vm_command {

	PopStatic::PopStatic(const std::string& file_name, uint16_t idx) {
		this->file_name = file_name;
		this->idx = idx;
	}

	void PopStatic::to_asm(std::ofstream &out) const {
        write_asm(out, "// pop static " + std::to_string(idx), "");
        write_asm(out, "@SP");
		write_asm(out, "AM=M-1");
		write_asm(out, "D=M");
		write_asm(out, "@" + file_name + "." + std::to_string(idx));
		write_asm(out, "M=D");
	}

}
