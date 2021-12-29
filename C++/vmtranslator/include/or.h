#ifndef __OR_H__
#define __OR_H__ 

#include <fstream>
#include "binary_alu_cmd.h"

namespace vm_command {

	class Or : public BinaryALUCommand {

		public:
			void to_asm(std::ofstream& out) const;
	};

}

#endif
