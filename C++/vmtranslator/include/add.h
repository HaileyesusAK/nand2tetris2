#ifndef __ADD_H__
#define __ADD_H__ 

#include <fstream>
#include "binary_alu_cmd.h"

namespace vm_command {
	
	class Add : public BinaryALUCommand {

		public:
			void to_asm(std::ofstream& out) const override;
	};

}

#endif
