#ifndef __EQ_H__
#define __EQ_H__

#include <fstream>
#include "relational_cmd.h"

namespace vm_command {

	class Eq : public RelCommand {

		public:
			void to_asm(std::ofstream &out) const;
	};
}

#endif
