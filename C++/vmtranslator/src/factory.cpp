#include <memory>
#include <string>
#include <sstream>
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
#include "factory.h"


namespace vm_command {

	std::unique_ptr<Command> Factory::create_instance(const std::string &command_line,
													  const std::string &file_name) {

		enum Command {ADD, SUB, AND, OR, EQ, LT, GT, NOT, NEG, PUSH, POP};
		const static std::unordered_map<std::string, Command> commands {
			{"add", ADD}, {"sub", SUB}, {"and", AND}, {"or", OR}, {"eq", EQ}, {"lt", LT},
			{"gt", GT}, {"not", NOT}, {"neg", NEG}, {"push", PUSH}, {"pop", POP},
		};

		std::istringstream iss {command_line};

		std::string cmd;
		iss >> cmd;
		if(!commands.count(cmd))
			throw std::invalid_argument(std::string("unknown command: ") + command_line);

		switch(commands.at(cmd)) {
			case ADD: return std::make_unique<Add>();
			case SUB: return std::make_unique<Sub>();
			case OR: return std::make_unique<Or>();
			case AND: return std::make_unique<And>();
			case EQ: return std::make_unique<Eq>();
			case GT: return std::make_unique<Gt>();
			case LT: return std::make_unique<Lt>();
			case NOT: return std::make_unique<Not>();
			case NEG: return std::make_unique<Neg>();
			case PUSH:
			case POP:
			{
				std::string arg1;
				iss >> arg1;
				if(iss.fail())
					throw std::invalid_argument(command_line + ": missing segment");
			
				uint16_t idx;
				iss >> idx;
				if(iss.fail())
					throw std::invalid_argument(command_line + ": missing index");

				if(commands.at(cmd) == PUSH) {
					if(arg1 == "static")
						return std::make_unique<PushStatic>(file_name, idx);
					else
						return std::make_unique<Push>(arg1, idx);
				}
				else {
					if(arg1 == "static")
						return std::make_unique<PopStatic>(file_name, idx);
					else
						return std::make_unique<Pop>(arg1, idx);
				}
			}
			default:
				return nullptr;
		}
	}

}
