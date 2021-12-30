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
#include "function.h"
#include "call.h"
#include "return.h"
#include "factory.h"


namespace vm_command {

	std::unique_ptr<Command> Factory::create_instance(const std::string &command_line,
													  const std::string &file_name) {
		enum Command {
            ADD, SUB, AND, OR, EQ, LT, GT, NOT, NEG, PUSH, POP,
            FUNCTION, CALL, RETURN
        };

		const static std::unordered_map<std::string, Command> commands {
			{"add", ADD}, {"sub", SUB}, {"and", AND}, {"or", OR}, {"eq", EQ}, {"lt", LT},
			{"gt", GT}, {"not", NOT}, {"neg", NEG}, {"push", PUSH}, {"pop", POP},
            {"function", FUNCTION}, {"call", CALL}, {"return", RETURN}
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
            case FUNCTION:
            case CALL:
			{
				std::string arg1;
				iss >> arg1;
				if(iss.fail())
					throw std::invalid_argument(command_line + ": missing segment");
			
				uint16_t arg2;
				iss >> arg2;
				if(iss.fail())
					throw std::invalid_argument(command_line + ": missing index");

                switch(commands.at(cmd)) {
				    case PUSH:
                    {
                        if(arg1 == "static")
                            return std::make_unique<PushStatic>(file_name, arg2);
                        else
                            return std::make_unique<Push>(arg1, arg2);
                    }
                    break;

                    case POP:
                    {
                        if(arg1 == "static")
                            return std::make_unique<PopStatic>(file_name, arg2);
                        else
                            return std::make_unique<Pop>(arg1, arg2);
                    }
                    break;
                    
                    case FUNCTION: return std::make_unique<Function>(arg1, arg2);
                    case CALL:  return std::make_unique<Call>(arg1, arg2);
                    default:
                        break;
				}
			}
            case RETURN: return std::make_unique<Return>();

			default:
				return nullptr;
		}
	}

}
