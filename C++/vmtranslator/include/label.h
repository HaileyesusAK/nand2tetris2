#ifndef __LABEL_H__
#define __LABEL_H__


#include <fstream>
#include <string>
#include "command.h"

namespace vm_command {

	class Label: public Command {
		private:
			std::string _function_name {};
			std::string _label {};

		public:
			Label(const std::string &function_name, const std::string &label)
				: _function_name(function_name), _label(label) {}

			void to_asm(std::ofstream &out) const override;
	};
}

#endif
