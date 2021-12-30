#ifndef __RELATIONAL_CMD_H__
#define __RELATIONAL_CMD_H__

#include <fstream>
#include "command.h"

namespace vm_command {

	/* A class to represent a generic binary relational vm command */
	class RelCommand : public Command {
		protected:
			/* writes to file assembly instructions necessary BEFORE any
			   translation of relational commands
			*/
			void prepend(std::ofstream &out) const;

			/* writes to file assembly instructions necessary AFTER any
			   translation of arithmetic and logical commands
			*/
			void append(std::ofstream &out) const;

			virtual ~RelCommand() = default;
	};

}

#endif
