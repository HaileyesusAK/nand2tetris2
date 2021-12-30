#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <fstream>
#include <string>

namespace vm_command {

	/* A class to represent a generic vm command */
	class Command {
		public:
			virtual ~Command() = default;

			/* writes in a file the command's translation to assembly instructions */
			virtual void to_asm(std::ofstream &out) const = 0;

			/* removes a comment section, and starting and trailing whitespaces
			   from a raw command
			*/
			static void compact(std::string &command);

			/* writes to a file <p>line</p> prefixed by the string <p>prefix</p>
			   and followed by a newline character
			*/
			static void write_asm(std::ofstream &out, const std::string &line, const std::string &prefix="\t");
	};

}

#endif

