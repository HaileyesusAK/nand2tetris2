#include <fstream>
#include <iostream>
#include "sub.h"

namespace vm_command {

	void Sub::to_asm(std::ofstream &out) const {
        write_asm(out, "// sub", "");
		BinaryALUCommand::prepend(out);
		write_asm(out, "M=M-D");
	}

}
