#include <fstream>
#include <iostream>
#include "or.h"

namespace vm_command {

	void Or::to_asm(std::ofstream &out) const {
        write_asm(out, "// or", "");
		BinaryALUCommand::prepend(out);
		write_asm(out, "M=M|D");
	}

}
