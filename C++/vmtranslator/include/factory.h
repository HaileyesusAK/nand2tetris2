#include <memory>
#include <string>
#include "command.h"

#include "add.h"
#include "sub.h"
#include "and.h"
#include "or.h"
#include "eq.h"
#include "lt.h"
#include "gt.h"
#include "not.h"
#include "neg.h"
#include "push.h"
#include "pop.h"
#include "push_static.h"
#include "pop_static.h"

namespace vm_command {

	/* A class used to generate objects of Command subclasses */
	class Factory {
		public:
			
			Factory(const std::string &file_name, const std::string &function_name = "")
				: _file_name(file_name), _function_name(function_name) {}

			/* Parses command_line and returns the right kind of Command object pointer. */
			std::unique_ptr<Command> create_instance(const std::string &command_line);

		private:
			std::string _file_name;
			std::string _function_name;
	};

}
