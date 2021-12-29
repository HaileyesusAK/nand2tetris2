#ifndef __GT_H__
#define __GT_H__

#include <fstream>
#include "relational_cmd.h"

namespace vm_command {

	class Gt : public RelCommand {

		public:
			void to_asm(std::ofstream &out) const;
	};
}

#endif
