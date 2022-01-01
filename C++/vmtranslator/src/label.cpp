#include <fstream>
#include <string>
#include "label.h"

namespace vm_command {

	void Label::to_asm(std::ofstream &out) const {
		write_asm(out, "(" + _function_name + "$" + _label + ")", "");
	}

}
