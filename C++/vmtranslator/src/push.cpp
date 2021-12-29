#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include "push.h"

namespace vm_command {

	Push::Push(const std::string& segment, uint16_t idx) {
		if(!Push::aliases.count(segment))
			throw std::invalid_argument(std::string("unknown segment: ") + segment);

		this->segment = segment;
		this->idx = idx;
	}

	void Push::to_asm(std::ofstream &out) const {
        write_asm(out, "// push " + segment + " " + std::to_string(idx), "");

		if(segment == "constant") {
			write_asm(out, "@" + std::to_string(idx));
			write_asm(out, "D=A");
		}
		else if(segment == "pointer" || segment == "temp") {
            // segment starts at address 3 for pointer and at address 5 for temp
            int addr = ((segment == "pointer") ? 3 : 5) + idx;
            write_asm(out, "@" + std::to_string(addr));
            write_asm(out, "D=M");
		}
		else {
			// Put the ith element from the segment in D
			write_asm(out, "@" + Push::aliases.at(segment));
			write_asm(out, "D=M");
			write_asm(out, "@" + std::to_string(idx));
			write_asm(out, "A=D+A");
			write_asm(out, "D=M");
		}

		// push on the stack the content of D
		write_asm(out, "@SP");
		write_asm(out, "A=M");
		write_asm(out, "M=D");
		write_asm(out, "@SP");
		write_asm(out, "M=M+1");
	}

}
