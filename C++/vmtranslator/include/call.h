#ifndef __CALL_H__
#define __CALL_H__

#include <fstream>
#include <string>
#include <vector>
#include "command.h"

namespace vm_command {

	using States = std::vector<std::string>;

	class Call : public Command {
		private:
			std::string function_name;
			uint16_t n_args;

		public:
			Call(const std::string &s, uint16_t n) : function_name(s), n_args(n) {}

			void to_asm(std::ofstream &out) const override;

			static const States& state_segments();

			static inline const uint16_t ARG_START = 5;
	};

}

#endif
