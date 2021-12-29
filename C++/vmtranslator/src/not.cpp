#include <fstream>
#include <iostream>
#include "not.h"

namespace vm_command {

	void Not::to_asm(std::ofstream &out) const {
        write_asm(out, "// not", "");
		write_asm(out, "@SP");
		write_asm(out, "A=M-1");
		write_asm(out, "M=!M");
	}

}
