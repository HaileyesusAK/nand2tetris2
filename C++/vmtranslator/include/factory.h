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
			/* parses command_line and returns the right kind of Command object pointer.
			   It requires the name of the vm file from which the command is read so that
			   it will be used in some Commands such as PushStatic.
			*/
			static std::unique_ptr<Command> create_instance(const std::string &command_line,
															const std::string &file_name);
	};

}
