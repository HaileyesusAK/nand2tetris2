#include <fstream>
#include <iostream>
#include "add.h"

namespace vm_command {

	void Add::to_asm(std::ofstream &out) const {
        write_asm(out, "// add", "");
		BinaryALUCommand::prepend(out);
		write_asm(out, "M=M+D");
	}

}
