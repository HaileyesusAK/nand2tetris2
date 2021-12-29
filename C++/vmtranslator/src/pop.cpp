#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include "pop.h"

namespace vm_command {

	Pop::Pop(const std::string& segment, uint16_t idx) {
		if(!Pop::aliases.count(segment))
			throw std::invalid_argument(std::string("unknown segment: ") + segment);

		this->segment = segment;
		this->idx = idx;
	}

	void Pop::to_asm(std::ofstream &out) const {
        write_asm(out, "// pop " + segment + " " + std::to_string(idx), "");

        if(segment == "pointer" || segment == "temp") {
            // pop the stack and store on register D
            write_asm(out, "@SP");
            write_asm(out, "AM=M-1");
            write_asm(out, "D=M");

            // segment starts at address 3 for pointer and at address 5 for temp
            int addr = ((segment == "pointer") ? 3 : 5) + idx;
            write_asm(out, "@" + std::to_string(addr));
            write_asm(out, "M=D");
        }
        else {
            // Move the segment's base pointer to the address of the ith element
            write_asm(out, "@" + std::to_string(idx));
            write_asm(out, "D=A");
            write_asm(out, "@" + Pop::aliases.at(segment));
            write_asm(out, "M=D+M");

            // pop the stack and store on register D
            write_asm(out, "@SP");
            write_asm(out, "AM=M-1");
            write_asm(out, "D=M");
        
            // copy register D in the ith element of the segment
            write_asm(out, "@" + Pop::aliases.at(segment));
            write_asm(out, "A=M");
            write_asm(out, "M=D");

            // Revert back the segment's base pointer to the start of the segment
            write_asm(out, "@" + std::to_string(idx));
            write_asm(out, "D=A");
            write_asm(out, "@" + Pop::aliases.at(segment));
            write_asm(out, "M=M-D");
        }
	}

}
