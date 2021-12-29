#ifndef __BINARY_ALU_H__
#define __BINARY_ALU_H__

#include <fstream>
#include "command.h"

namespace vm_command {

	/* A generic binary ALU vm command */
	class BinaryALUCommand : public Command {

		protected:
			void prepend(std::ofstream &out) const;
			virtual ~BinaryALUCommand() = default;
	};

}

#endif
