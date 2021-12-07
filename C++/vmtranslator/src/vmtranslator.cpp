#include <filesystem>
#include <functional>
#include <fstream>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <variant>
#include <vector>

#include "vmtranslator.h"


using namespace std::placeholders;

using BinaryAluTranslatorType = InstList (VmTranslator::*)(const BinaryAluOp&);
auto sub_translator = std::bind((BinaryAluTranslatorType)(&VmTranslator::translate), _1, BinaryAluOp::SUB);
auto add_translator = std::bind((BinaryAluTranslatorType)(&VmTranslator::translate), _1, BinaryAluOp::ADD);
auto and_translator = std::bind((BinaryAluTranslatorType)(&VmTranslator::translate), _1, BinaryAluOp::AND);
auto or_translator = std::bind((BinaryAluTranslatorType)(&VmTranslator::translate), _1, BinaryAluOp::OR);

using RelAluTranslatorType = InstList (VmTranslator::*)(const RelOp&);
auto lt_translator = std::bind((RelAluTranslatorType)(&VmTranslator::translate), _1, RelOp::LT);
auto eq_translator = std::bind((RelAluTranslatorType)(&VmTranslator::translate), _1, RelOp::EQ);
auto gt_translator = std::bind((RelAluTranslatorType)(&VmTranslator::translate), _1, RelOp::GT);

using UnaryAluTranslatorType = InstList (VmTranslator::*)(const UnaryOp&);
auto not_translator = std::bind((UnaryAluTranslatorType)(&VmTranslator::translate), _1, UnaryOp::NOT);
auto neg_translator = std::bind((UnaryAluTranslatorType)(&VmTranslator::translate), _1, UnaryOp::NEG);

using NoParamTranslator = InstList (VmTranslator::*)();
using DoubleParamTranslator = InstList (VmTranslator::*)(const std::string&, uint16_t);

using Translator = std::variant<NoParamTranslator,
							    DoubleParamTranslator,
								decltype(sub_translator),
								decltype(lt_translator),
								decltype(not_translator)>;

struct TranslatorVisitor {
    VmTranslator* translator_ptr;

    TranslatorVisitor(VmTranslator* obj_ptr) : translator_ptr(obj_ptr) {}

    InstList operator()(NoParamTranslator f) {
        return (translator_ptr->*f)();
    }

    InstList operator()(DoubleParamTranslator f) {
        return (translator_ptr->*f)(translator_ptr->current_command.arg1,
									translator_ptr->current_command.arg2);
    }
	
    InstList operator()(decltype(sub_translator) f) {
        return f(translator_ptr);
	}

    InstList operator()(decltype(lt_translator) f) {
        return f(translator_ptr);
	}

    InstList operator()(decltype(not_translator) f) {
        return f(translator_ptr);
	}
};

void VmTranslator::set_filename(const std::string& filename) {
	file_name = filename;
}

InstList VmTranslator::translate(const BinaryAluOp& op) {
    InstList inst {"@SP", "AM=M-1", "D=M", "@SP", "A=M-1"};

    switch(op) {
        case BinaryAluOp::ADD: inst.push_back("M=M+D"); break;
        case BinaryAluOp::SUB: inst.push_back("M=M-D"); break;
        case BinaryAluOp::AND: inst.push_back("M=D&M"); break;
        case BinaryAluOp::OR:  inst.push_back("M=D|M"); break;
    }

    return inst;
}

InstList VmTranslator::translate(const RelOp& op) {
    InstList inst { "@SP", "AM=M-1", "D=M", "@SP", "A=M-1", "D=M-D"};
    inst.push_back("M=-1");     //Assume the arguments are equal

    // the next instruction address to execute immediately if the arguments are not equal
    uint16_t pc = program_counter + inst.size() + 5;
    inst.push_back("@" + std::to_string(pc));

    switch(op) {
        case RelOp::EQ: inst.push_back("D;JEQ"); break;
        case RelOp::GT: inst.push_back("D;JGT"); break;
        case RelOp::LT: inst.push_back("D;JLT"); break;
    }

    inst.push_back("@SP");
    inst.push_back("A=M-1");
    inst.push_back("M=0");

    return inst;
}

InstList VmTranslator::translate(const UnaryOp& op) {
    InstList inst { "@SP", "A=M-1" };
    if(op == UnaryOp::NEG)
        inst.push_back("M=-M");
    else
        inst.push_back("M=!M");

    return inst;
}

void VmTranslator::append_push_D(InstList& instructions) {
    const static InstList push_D_instructions {
        "@SP", "A=M", "M=D", "@SP", "M=M+1"
    };

    for(const auto& inst : push_D_instructions)
        instructions.push_back(inst);
}

void VmTranslator::append_pop_D(InstList& instructions) {
    const static InstList push_D_instructions {
        "@SP", "AM=M-1", "D=M"
    };

    for(const auto& inst : push_D_instructions)
        instructions.push_back(inst);
}

InstList VmTranslator::translate_push(const PushableSegment& segment, uint16_t idx) {
    const static std::unordered_map<PushableSegment, std::string> aliases {
        {PushableSegment::ARG, "ARG"},
        {PushableSegment::LCL, "LCL"},
        {PushableSegment::THIS, "THIS"},
        {PushableSegment::THAT, "THAT"},
        {PushableSegment::POINTER, "R3"},
        {PushableSegment::TEMP, "R5"},
        {PushableSegment::STATIC, ""},	// Static and Constant segment do not have alias symbols
        {PushableSegment::CONSTANT, ""}
    };

    InstList instructions;
	switch(segment) {
		case PushableSegment::STATIC:
			instructions.push_back("@" + file_name + "." + std::to_string(idx));
			instructions.push_back("D=M");
		break;

		case PushableSegment::CONSTANT:
			instructions.push_back("@" + std::to_string(idx));
			instructions.push_back("D=A");
		break;
		
		default:
			// Put the ith element from the segment in D
			instructions.push_back(	"@" + aliases.at(segment));
			instructions.push_back("D=M");
			instructions.push_back("@" + std::to_string(idx));
			instructions.push_back("A=D+A");
			instructions.push_back("D=M");
		break;
	}

    append_push_D(instructions);

    return instructions;
}

InstList VmTranslator::translate_pop(const PopableSegment& segment, uint16_t idx) {
    const static std::unordered_map<PopableSegment, std::string> aliases {
        {PopableSegment::ARG, "ARG"},
        {PopableSegment::LCL, "LCL"},
        {PopableSegment::THIS, "THIS"},
        {PopableSegment::THAT, "THAT"},
        {PopableSegment::POINTER, "R3"},
        {PopableSegment::TEMP, "R5"},
        {PopableSegment::STATIC, ""}	// Static segment does not have alias symbol
    };

	InstList instructions;

	if(segment == PopableSegment::STATIC) {
		append_pop_D(instructions);
		instructions.push_back("@" + file_name + "." + std::to_string(idx));
		instructions.push_back("M=D");
	}
	else {
		// Move the segment's base pointer to the address of the ith element
		instructions.push_back("@" + std::to_string(idx));
		instructions.push_back("D=A");
		instructions.push_back("@" + aliases.at(segment));
		instructions.push_back("M=D+M");

		// Push D onto to the ith element
		append_pop_D(instructions);
		instructions.push_back("@" + aliases.at(segment));
		instructions.push_back("A=M");
		instructions.push_back("M=D");

		// Move the segment's base pointer to the start of the segment
		instructions.push_back("@" + std::to_string(idx));
		instructions.push_back("D=A");
		instructions.push_back("@" + aliases.at(segment));
		instructions.push_back("M=M-D");
	}

    return instructions;
}

InstList VmTranslator::translate_push(const std::string& segment, uint16_t idx) {
	const static std::unordered_map<std::string, PushableSegment> segments {
		{"constant", PushableSegment::CONSTANT},
		{"static", PushableSegment::STATIC},
		{"local", PushableSegment::LCL},
		{"argument", PushableSegment::ARG},
		{"this", PushableSegment::THIS},
		{"that", PushableSegment::THAT},
		{"temp", PushableSegment::TEMP},
		{"pointer", PushableSegment::POINTER}
	};

	return translate_push(segments.at(segment), idx);
}

InstList VmTranslator::translate_pop(const std::string& segment, uint16_t idx) {
	const static std::unordered_map<std::string, PopableSegment> segments {
		{"static", PopableSegment::STATIC},
		{"local", PopableSegment::LCL},
		{"argument", PopableSegment::ARG},
		{"this", PopableSegment::THIS},
		{"that", PopableSegment::THAT},
		{"temp", PopableSegment::TEMP},
		{"pointer", PopableSegment::POINTER}
	};

	return translate_pop(segments.at(segment), idx);
}

bool VmTranslator::parse_command(const std::string& vm_cmd) {
	const static std::set<std::string> commands {
		"add", "sub", "and", "or", "eq", "lt", "gt", "not", "neg",
		"push", "pop"
	};

	const static std::set<std::string> segments {
		"constant", "static", "local", "argument", "this", "that", "temp", "pointer"
	};

	std::istringstream iss {vm_cmd};
	std::string cmd;
	std::string arg1;
	uint16_t arg2;

	iss >> cmd;
	if(!commands.count(cmd))
		return false;

	if(cmd == "push" || cmd == "pop") {
		iss >> arg1;
	
		// the segment must be known
		if(!segments.count(arg1))
			return false;

		// constant doesn't have a physical segment
		if(cmd == "pop" && arg1 == "constant")
			return false;

		// attempt to read the index
		iss >> arg2;
		if(iss.fail())
			return false;
	}

	current_command.cmd = cmd;
	current_command.arg1 = arg1;
	current_command.arg2 = arg2;
	
	return true;
}

void VmTranslator::translate(const fs::path& vm_file_path) {
    if(!fs::exists(vm_file_path))
        throw std::runtime_error("Input file doesn't exist");

    static std::unordered_map<std::string, Translator> translators {
        {"add", add_translator},
        {"sub", sub_translator},
        {"and", and_translator},
        {"or", or_translator},
        {"eq", eq_translator},
        {"lt", lt_translator},
        {"gt", gt_translator},
        {"not", not_translator},
        {"neg", neg_translator},
		{"push", (DoubleParamTranslator)(&VmTranslator::translate_push)},
		{"pop", (DoubleParamTranslator)(&VmTranslator::translate_pop)}
    };

    file_name = vm_file_path.filename();
    auto asm_file_path = vm_file_path;
    asm_file_path.replace_extension(".asm");

    std::ifstream ifs {vm_file_path};
    std::ofstream ofs {asm_file_path};

    std::string line;
    InstList asm_instructions;

	TranslatorVisitor visitor {this};

    while(!ifs.eof()) {
        getline(ifs, line);
        if(line.empty())
            continue;

		if(!parse_command(line))
			throw std::runtime_error("Unknown command: " + line);

		asm_instructions = std::visit(visitor, translators.at(current_command.cmd));

        std::copy(asm_instructions.begin(),
                  asm_instructions.end(),
                  std::ostream_iterator<std::string>(ofs, "\n"));
        program_counter += asm_instructions.size();
    }
}
