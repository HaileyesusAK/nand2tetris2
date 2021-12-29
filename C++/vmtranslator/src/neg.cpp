#include <fstream>
#include <iostream>
#include "neg.h"

namespace vm_command {

	void Neg::to_asm(std::ofstream &out) const {
        write_asm(out, "// neg", "");
		write_asm(out, "@SP");
		write_asm(out, "A=M-1");
		write_asm(out, "M=-M");
	}

}
