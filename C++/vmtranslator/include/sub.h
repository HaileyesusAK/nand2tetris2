#ifndef __SUB_H__
#define __SUB_H__ 

#include <fstream>
#include "binary_alu_cmd.h"

namespace vm_command {

	class Sub : public BinaryALUCommand {

		public:
			void to_asm(std::ofstream& out) const;
	};

}

#endif
