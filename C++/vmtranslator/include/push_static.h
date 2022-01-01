#ifndef __PUSH_STATIC_H__
#define __PUSH_STATIC_H__

#include <fstream>
#include <string>
#include "command.h"

namespace vm_command {

	class PushStatic : public Command {

		private:
			std::string file_name;
			uint16_t idx;

		public:
			PushStatic(const std::string& file_name, uint16_t idx);
			void to_asm(std::ofstream &out) const override;
	};

}

#endif
