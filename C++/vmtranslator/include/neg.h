#ifndef __NEG_H__
#define __NEG_H__

#include <fstream>
#include "command.h"

namespace vm_command {

	class Neg : public Command {
		
		public:
			void to_asm(std::ofstream &out) const override;
	};
}

#endif
