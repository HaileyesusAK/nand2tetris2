#include <fstream>
#include <string>
#include "goto.h"

namespace vm_command {

	void Goto::to_asm(std::ofstream &out) const {
        write_asm(out, "// goto " + _label, "");

    	write_asm(out, "@" + _function_name + "$" + _label );
    	write_asm(out, "0;JMP");
	}

}
