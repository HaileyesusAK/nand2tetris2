#ifndef __NOT_H__
#define __NOT_H__

#include <fstream>
#include "command.h"

namespace vm_command {

	class Not : public Command {
		
		public:
			void to_asm(std::ofstream &out) const;
	};
}

#endif
