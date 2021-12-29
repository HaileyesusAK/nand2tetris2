#ifndef __AND_H__
#define __AND_H__ 

#include <fstream>
#include "binary_alu_cmd.h"

namespace vm_command {

	class And : public BinaryALUCommand {

		public:
			void to_asm(std::ofstream& out) const;
	};

}

#endif
