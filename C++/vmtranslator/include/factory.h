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

	class Factory {

		public:
			static std::unique_ptr<Command> create_instance(const std::string &command_line,
															const std::string &file_name);
	};

}
