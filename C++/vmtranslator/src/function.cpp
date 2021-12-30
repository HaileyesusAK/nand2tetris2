#include <fstream>
#include <string>
#include "command.h"
#include "function.h"

namespace vm_command {

	void Function::to_asm(std::ofstream &out) const {
		/*   
			translating function declaration command entails two things:
				1. creating a label (f_name)
				2. pushing 0 on the local segment n_locals times
		*/

		write_asm(out, "// function " + name + " " + std::to_string(n_locals), "");

		// label the function's starting point
		write_asm(out, "(" + name + ")");

		//Setup loop variables
		write_asm(out, "@LCL");
		write_asm(out, "D=M");
		write_asm(out, "@i");
		write_asm(out, "M=D");
		write_asm(out, "@" + std::to_string(n_locals));
		write_asm(out, "D=D+A");
		write_asm(out, "@n");
		write_asm(out, "M=D");

		// begin loop
		std::string loop { name + "_SET_LCL" };
		write_asm(out, "(" + loop +")");

		// check for loop termination condition
		std::string end { name + "_SET_LCL_END" };
		write_asm(out, "@n");
		write_asm(out, "D=M");
		write_asm(out, "@i");
		write_asm(out, "D=M-D");
		write_asm(out, "@" + end );
		write_asm(out, "D;JGE");

		// push 0 onto the local segment
		write_asm(out, "@i");
		write_asm(out, "A=M");
		write_asm(out, "M=0");

		// increment SP and loop variable, and goto the beginning
		write_asm(out, "@i");
		write_asm(out, "M=M+1");
		write_asm(out, "@SP");
		write_asm(out, "M=M+1");
		write_asm(out, "@" + loop );
		write_asm(out, "0;JMP");

		// end loop
		write_asm(out, "(" + end + ")");
	}

}
