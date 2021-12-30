#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <fstream>
#include <string>
#include "command.h"

namespace vm_command {

	class Function : public Command {
		private:
			std::string name;
			uint16_t n_locals;

		public:
			Function(const std::string &s, uint16_t n) : name(s), n_locals(n) {}

			void to_asm(std::ofstream &out) const;
	};

}

#endif
