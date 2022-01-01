#include <fstream>
#include <string>
#include "if-goto.h"

namespace vm_command {

	void IfGoto::to_asm(std::ofstream &out) const {
        write_asm(out, "// if-goto " + _label, "");

    	write_asm(out, "@SP");
    	write_asm(out, "AM=M-1");
    	write_asm(out, "D=M");
    	write_asm(out, "@" + _function_name + "$" + _label );
    	write_asm(out, "D;JNE");
	}

}
