#include <fstream>
#include <string>
#include "command.h"
#include "call.h"

namespace vm_command {

	const States& Call::state_segments() {
        static const States STATE_SEGMENTS {"LCL", "ARG", "THIS", "THAT"};
		return STATE_SEGMENTS;
	}

	void Call::to_asm(std::ofstream &out) const {
		/*
			push return-address
			push LCL
			push ARG
			push THIS
			push THAT
			ARG = SP - n_args - 5
			LCL = SP
			goto function_name
			(return_address)
		*/

		write_asm(out, "// call " + function_name + " " + std::to_string(n_args));

		static int n_calls = 1;
		std::string ret_addr = function_name + "$end" + std::to_string(n_calls++);

		// save return address
		write_asm(out, "@" + ret_addr);
		write_asm(out, "D=A");
		write_asm(out, "@SP");
		write_asm(out, "A=M");
		write_asm(out, "M=D");
		write_asm(out, "@SP");
		write_asm(out, "M=M+1");

		// save caller's state
		for(const auto& segment: Call::state_segments()) {
			write_asm(out, "@" + segment );
			write_asm(out, "D=M");
			write_asm(out, "@SP");
			write_asm(out, "A=M");
			write_asm(out, "M=D");
			write_asm(out, "@SP");
			write_asm(out, "M=M+1");
		}

		// setup ARG for the callee
		write_asm(out, "@" + std::to_string(Call::ARG_START) );
		write_asm(out, "D=A");
		write_asm(out, "@" + std::to_string(n_args));
		write_asm(out, "D=D+A");
		write_asm(out, "@SP");
		write_asm(out, "D=M-D");
		write_asm(out, "@ARG");
		write_asm(out, "M=D");

		// setup LCL for the callee
		write_asm(out, "@SP");
		write_asm(out, "D=M");
		write_asm(out, "@LCL");
		write_asm(out, "M=D");

		// jump to the called function
		write_asm(out, "@" + function_name);
		write_asm(out, "0;JMP");

		// label end of function
		write_asm(out, "(" + ret_addr + ")");
	}

}
