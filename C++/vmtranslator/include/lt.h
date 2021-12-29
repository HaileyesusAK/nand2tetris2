#ifndef __LT_H__
#define __LT_H__

#include <fstream>
#include "relational_cmd.h"

namespace vm_command {

	class Lt : public RelCommand {

		public:
			void to_asm(std::ofstream &out) const;
	};
}

#endif
