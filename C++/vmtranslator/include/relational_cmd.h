#ifndef __RELATIONAL_CMD_H__
#define __RELATIONAL_CMD_H__

#include <fstream>
#include "command.h"

namespace vm_command {

	class RelCommand : public Command {

		protected:
			void prepend(std::ofstream &out) const;
			void append(std::ofstream &out) const;
			virtual ~RelCommand() = default;
	};

}

#endif
