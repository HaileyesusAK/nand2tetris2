#include <memory>
#include <string>
#include <sstream>
#include "command_types.h"
#include "factory.h"


namespace vm_command {

	std::unique_ptr<Command> Factory::create_instance(const std::string &command_line) {
		enum Command {
            ADD, SUB, AND, OR, EQ, LT, GT, NOT, NEG, PUSH, POP,
            FUNCTION, CALL, RETURN, LABEL, GOTO, IF_GOTO
        };

		const static std::unordered_map<std::string, Command> commands {
			{"add", ADD}, {"sub", SUB}, {"and", AND}, {"or", OR}, {"eq", EQ}, {"lt", LT},
			{"gt", GT}, {"not", NOT}, {"neg", NEG}, {"push", PUSH}, {"pop", POP},
            {"function", FUNCTION}, {"call", CALL}, {"return", RETURN},
            {"label", LABEL}, {"goto", GOTO}, {"if-goto", IF_GOTO}
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
                            return std::make_unique<PushStatic>(_file_name, arg2);
                        else
                            return std::make_unique<Push>(arg1, arg2);
                    }
                    break;

                    case POP:
                    {
                        if(arg1 == "static")
                            return std::make_unique<PopStatic>(_file_name, arg2);
                        else
                            return std::make_unique<Pop>(arg1, arg2);
                    }
                    break;
                    
                    case FUNCTION:
					{
						_function_name = arg1;
						return std::make_unique<Function>(_function_name, arg2);
					}

                    case CALL:  return std::make_unique<Call>(arg1, arg2);
                    default:
                        break;
				}
			}
            case RETURN: return std::make_unique<Return>();
			case LABEL:
			case GOTO:
			case IF_GOTO:
			{
				std::string label;
				iss >> label;
				if(iss.fail())
					throw std::invalid_argument(command_line + ": missing label");

                switch(commands.at(cmd)) {
                    case LABEL: return std::make_unique<Label>(_function_name, label);
                    case GOTO: return std::make_unique<Goto>(_function_name, label);
                    case IF_GOTO: return std::make_unique<IfGoto>(_function_name, label);
                    default:
                        break;
                }
			}

			default:
				return nullptr;
		}
	}

}
