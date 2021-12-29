#include <algorithm>
#include <iostream>
#include <string>
#include "command.h"

namespace vm_command {

	void Command::compact(std::string &command)  {
		// remove trailing comment
		auto comment_begin = command.find("//");
		if(comment_begin != std::string::npos)
			command.erase(comment_begin);

		// erase beginning non alphabetic characters
		auto alpha_begin = std::find_if(command.begin(), command.end(), ::isalpha);
		if(alpha_begin != command.end())
			command.erase(command.begin(), alpha_begin);

		// erase trailing white spaces
        auto alpha_end = std::find_if(command.rbegin(), command.rend(), ::isalnum);
        std::string::iterator end = alpha_end.base();
        command.erase(end, command.end());
	}

	void Command::write_asm(std::ofstream &out, const std::string &line, const std::string &prefix) {
		out << prefix << line << std::endl;
	}
}
