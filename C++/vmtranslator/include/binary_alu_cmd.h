#ifndef __BINARY_ALU_H__
#define __BINARY_ALU_H__

#include <fstream>
#include "command.h"

namespace vm_command {

	/* A class to represent a generic binary arithmetic and logical vm command */
	class BinaryALUCommand : public Command {
		protected:
			/* writes to file assembly instructions necessary before any
			   translation of arithmetic and logical commands
			*/
			void prepend(std::ofstream &out) const;

			virtual ~BinaryALUCommand() = default;
	};

}

#endif
