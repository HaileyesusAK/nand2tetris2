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


const Translators& VmTranslator::get_translators() {
	using BinaryAluTranslator = void (VmTranslator::*)(const BinaryAluOp&);
	using RelAluTranslator = void (VmTranslator::*)(const RelOp&);
	using UnaryAluTranslator = void (VmTranslator::*)(const UnaryOp&);
	using namespace std::placeholders;

    static const Translators translators {
		{"add", std::bind((BinaryAluTranslator)(&VmTranslator::translate), _1, BinaryAluOp::ADD)},
		{"sub", std::bind((BinaryAluTranslator)(&VmTranslator::translate), _1, BinaryAluOp::SUB)},
		{"and", std::bind((BinaryAluTranslator)(&VmTranslator::translate), _1, BinaryAluOp::AND)},
		{"or", std::bind((BinaryAluTranslator)(&VmTranslator::translate), _1, BinaryAluOp::OR)},
		{"lt", std::bind((RelAluTranslator)(&VmTranslator::translate), _1, RelOp::LT)},
		{"gt", std::bind((RelAluTranslator)(&VmTranslator::translate), _1, RelOp::GT)},
		{"eq", std::bind((RelAluTranslator)(&VmTranslator::translate), _1, RelOp::EQ)},
		{"not", std::bind((UnaryAluTranslator)(&VmTranslator::translate), _1, UnaryOp::NOT)},
		{"neg", std::bind((UnaryAluTranslator)(&VmTranslator::translate), _1, UnaryOp::NEG)},
		{"push", &VmTranslator::translate_push},
		{"pop", &VmTranslator::translate_pop}
    };

	return translators;
}

void VmTranslator::operator()(const DoubleParamTranslator& translator) {
    (this->*translator)(current_command.arg1, current_command.arg2);
}

void VmTranslator::operator()(const BoundAluTranslator& translator) {
        translator(this);
}

void VmTranslator::translate(const BinaryAluOp& op) {
	out << "@SP" << std::endl;
	out << "AM=M-1" << std::endl;
	out << "D=M" << std::endl;
	out << "@SP" << std::endl;
	out << "A=M-1" << std::endl;

    switch(op) {
        case BinaryAluOp::ADD: out << "M=M+D" << std::endl; break;
        case BinaryAluOp::SUB: out << "M=M-D" << std::endl; break;
        case BinaryAluOp::AND: out << "M=D&M" << std::endl; break;
        case BinaryAluOp::OR:  out << "M=D|M" << std::endl; break;
    }
}

void VmTranslator::translate(const RelOp& op) {
	out << "@SP" << std::endl;
	out << "AM=M-1" << std::endl;
	out << "D=M" << std::endl;
	out << "@SP" << std::endl;
	out << "A=M-1" << std::endl;
	out << "D=M-D" << std::endl;
	out << "M=-1" << std::endl;     //Assume the arguments are equal

    // the next instruction address to execute immediately if the arguments are not equal
    uint16_t pc = program_counter + 7 + 5;
	out << "@" << pc << std::endl;

    switch(op) {
        case RelOp::EQ: out << "D;JEQ" << std::endl; break;
        case RelOp::GT: out << "D;JGT" << std::endl; break;
        case RelOp::LT: out << "D;JLT" << std::endl; break;
    }

    out << "@SP" << std::endl;
	out << "A=M-1" << std::endl;
    out << "M=0" << std::endl;
}

void VmTranslator::translate(const UnaryOp& op) {
	out << "@SP" << std::endl;
	out << "A=M-1" << std::endl;

    if(op == UnaryOp::NEG)
		out << "M=-M" << std::endl;
    else
		out << "M=!M" << std::endl;
}

void VmTranslator::translate(const PushableSegment& segment, uint16_t idx) {
    const static std::unordered_map<PushableSegment, std::string> aliases {
        {PushableSegment::ARG, "ARG"},
        {PushableSegment::LCL, "LCL"},
        {PushableSegment::THIS, "THIS"},
        {PushableSegment::THAT, "THAT"},
        {PushableSegment::POINTER, "R3"},
        {PushableSegment::TEMP, "R5"},
        {PushableSegment::STATIC, ""},	// Static and Constant segment do not have aliases
        {PushableSegment::CONSTANT, ""}
    };

	switch(segment) {
		case PushableSegment::STATIC:
			out << "@" << file_name << "." << idx << std::endl;
			out << "D=M" << std::endl;
		break;

		case PushableSegment::CONSTANT:
			out << "@" << idx << std::endl;
			out << "D=A" << std::endl;
		break;
		
		default:
			// Put the ith element from the segment in D
			out << "@" << aliases.at(segment) << std::endl;
			out << "D=M" << std::endl;
			out << "@" << idx << std::endl;
			out << "A=D+A" << std::endl;
			out << "D=M" << std::endl;
		break;
	}
    
	out << "@SP" << std::endl;
	out << "A=M" << std::endl;
	out << "M=D" << std::endl;
	out << "@SP" << std::endl;
	out << "M=M+1" << std::endl;
}

void VmTranslator::translate(const PopableSegment& segment, uint16_t idx) {
    const static std::unordered_map<PopableSegment, std::string> aliases {
        {PopableSegment::ARG, "ARG"},
        {PopableSegment::LCL, "LCL"},
        {PopableSegment::THIS, "THIS"},
        {PopableSegment::THAT, "THAT"},
        {PopableSegment::POINTER, "R3"},
        {PopableSegment::TEMP, "R5"},
        {PopableSegment::STATIC, ""}	// Static segment does not have alias symbol
    };

	if(segment == PopableSegment::STATIC) {
        out << "@SP" << std::endl;
		out << "AM=M-1" << std::endl;
		out << "D=M" << std::endl;
		out << "@" << file_name << "." << idx << std::endl;
		out << "M=D" << std::endl;
	}
	else {
		// Move the segment's base pointer to the address of the ith element
		out << "@" << idx << std::endl;
		out << "D=A" << std::endl;
		out << "@" << aliases.at(segment) << std::endl;
		out << "M=D+M" << std::endl;

		// Push D onto to the ith element
        out << "@SP" << std::endl;
		out << "AM=M-1" << std::endl;
		out << "D=M" << std::endl;
		out << "@" << aliases.at(segment) << std::endl;
		out << "A=M" << std::endl;
		out << "M=D" << std::endl;

		// Revert back the segment's base pointer to the start of the segment
		out << "@" << idx << std::endl;
		out << "D=A" << std::endl;
		out << "@" << aliases.at(segment) << std::endl;
		out << "M=M-D" << std::endl;
	}
}

void VmTranslator::translate_push(const std::string& segment, uint16_t idx) {
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

	translate(segments.at(segment), idx);
}

void VmTranslator::translate_pop(const std::string& segment, uint16_t idx) {
	const static std::unordered_map<std::string, PopableSegment> segments {
		{"static", PopableSegment::STATIC},
		{"local", PopableSegment::LCL},
		{"argument", PopableSegment::ARG},
		{"this", PopableSegment::THIS},
		{"that", PopableSegment::THAT},
		{"temp", PopableSegment::TEMP},
		{"pointer", PopableSegment::POINTER}
	};

	translate(segments.at(segment), idx);
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

	current_command = {cmd, arg1, arg2};
	
	return true;
}

void VmTranslator::translate(const fs::path& vm_file_path) {
    if(!fs::exists(vm_file_path))
        throw std::runtime_error("Input file doesn't exist");

    auto translators = VmTranslator::get_translators();

    file_name = vm_file_path.filename();
    auto asm_file_path = vm_file_path;
    asm_file_path.replace_extension(".asm");
	out = asm_file_path;

    std::string line;
    std::ifstream ifs {vm_file_path};
    while(!ifs.eof()) {
        getline(ifs, line);
        if(line.empty())
            continue;

		if(!parse_command(line))
			throw std::runtime_error("Unknown command: " + line);

		std::visit(*this, translators.at(current_command.cmd));
        program_counter = static_cast<uint16_t>(out.tellp());
    }
}
