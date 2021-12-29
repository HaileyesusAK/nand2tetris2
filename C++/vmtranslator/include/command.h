#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <fstream>
#include <string>

/* A generic vm command */

namespace vm_command {

	class Command {

		public:
			virtual ~Command() = default;
			virtual void to_asm(std::ofstream &out) const = 0;
			static void compact(std::string &command);
			static void write_asm(std::ofstream &out, const std::string &line, const std::string &prefix="\t");
	};

}

#endif

