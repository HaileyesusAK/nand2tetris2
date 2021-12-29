#include <fstream>
#include <iostream>
#include "and.h"

namespace vm_command {

	void And::to_asm(std::ofstream &out) const {
        write_asm(out, "// and", "");
		BinaryALUCommand::prepend(out);
		write_asm(out, "M=M&D");
	}

}
