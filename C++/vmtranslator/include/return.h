#ifndef __RETURN_H__
#define __RETURN_H__

#include <fstream>
#include "command.h"

namespace vm_command {
	
	class Return : public Command {

		public:
			void to_asm(std::ofstream &out) const override;
	};
}

#endif
