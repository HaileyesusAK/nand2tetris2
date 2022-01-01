#ifndef __POP_H__
#define __POP_H__

#include <fstream>
#include <string>
#include <unordered_map>
#include "command.h"

namespace vm_command {

	class Pop : public Command {

		private:
			const static inline std::unordered_map<std::string, std::string> aliases {
				{"argument", "ARG"},
				{"local", "LCL"},
				{"this", "THIS"},
				{"that", "THAT"},
				{"pointer", ""},
				{"temp", ""},
			};
			std::string segment;
			uint16_t idx;

		public:
			Pop(const std::string& segment, uint16_t idx);
			void to_asm(std::ofstream &out) const override;
	};

}

#endif
