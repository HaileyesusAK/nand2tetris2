#include <fstream>
#include <string>
#include <vector>
#include "command.h"
#include "call.h"
#include "return.h"

namespace vm_command {

	void Return::to_asm(std::ofstream &out) const {
		/*
			frame = LCL
			ret = *(frame - 5)
			*ARG = pop()
			SP = ARG + 1
			THAT = *(frame - 1)
			THIS = *(frame - 2)
			ARG = *(frame - 3)
			LCL = *(frame - 4)

			goto ret
		*/

		write_asm(out, "// return", "");
		
		// save the current function's LCL in a variable
		write_asm(out, "@LCL");
		write_asm(out, "D=M");
		write_asm(out, "@frame");
		write_asm(out, "M=D");

		// save the return address in a variable
		write_asm(out, "@" + std::to_string(Call::ARG_START));
		write_asm(out, "D=A");
		write_asm(out, "@frame");
		write_asm(out, "A=M-D");
		write_asm(out, "D=M");
		write_asm(out, "@ret");
		write_asm(out, "M=D");

		// return value to the caller
		write_asm(out, "@SP");
		write_asm(out, "A=M-1");
		write_asm(out, "D=M");
		write_asm(out, "@ARG");
		write_asm(out, "A=M");
		write_asm(out, "M=D");

		// update SP
		write_asm(out, "@ARG");
		write_asm(out, "D=M+1");
		write_asm(out, "@SP");
		write_asm(out, "M=D");

		// reset caller's state in reverse order
		for(auto it = Call::state_segments().crbegin(); it != Call::state_segments().crend(); ++it) {
			write_asm(out, "@frame");
			write_asm(out, "AM=M-1");
			write_asm(out, "D=M");
			write_asm(out, "@" + *it);
			write_asm(out, "M=D");
		}

		// return control to the caller
		write_asm(out, "@ret");
		write_asm(out, "A=M");
		write_asm(out, "0;JMP");
	}

}
