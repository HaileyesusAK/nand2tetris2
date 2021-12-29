#ifndef __PUSH_H__
#define __PUSH_H__

#include <fstream>
#include <string>
#include <unordered_map>
#include "command.h"

namespace vm_command {

	class Push : public Command {

		private:
			const static inline std::unordered_map<std::string, std::string> aliases {
				{"argument", "ARG"},
				{"local", "LCL"},
				{"this", "THIS"},
				{"that", "THAT"},
				{"pointer", "R3"},
				{"temp", "R5"},
				{"constant", ""} // constant segment does not have an alias
			};
			std::string segment;
			uint16_t idx;

		public:
			Push(const std::string& segment, uint16_t idx);
			void to_asm(std::ofstream &out) const ;
	};

}

#endif
