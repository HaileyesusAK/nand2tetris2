#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include "instruction.h"

std::string Instruction::compact(const std::string& instruction)  {
    std::string s = instruction.substr(0, instruction.find("//"));
    auto end = std::remove_if(s.begin(), s.end(), ::isspace);
    s.erase(end, s.end());
    return s;
}

const std::string& Instruction::peek_value() const { return value; }

const InstructionType& Instruction::type() const {
	static constexpr InstructionType type = InstructionType::UNKNOWN;
	return type;
}

/*
	Attempt to construct a CInstruction from a string

	A c-instruction can be in either of the following form:
		COMP
		DST=COMP
		COMP;JMP
		DST=COMP;JMP
		
		where,
			COMP: a valid comp part
			DST: a valid destination part
			JMP: a valid jump part
	
	If the string is not formated as such, the construction fails
	by throwing an appropriate exception.
*/

CInstruction::CInstruction(const std::string& c_instruction) {
	if(c_instruction.empty())
		throw std::runtime_error(std::string("empty instruction"));

	// attempt to extract dst part
    auto it = c_instruction.find('=');
    if(it != std::string::npos) {
		dst = c_instruction.substr(0, it);
		if(!dst_encodings.count(dst))
			throw InvalidDstErr(c_instruction);
	}

	// attemp to extract comp part
    auto it1 = c_instruction.find('=');
    auto it2 = c_instruction.find(';');

    if(it1 == std::string::npos && it2 == std::string::npos) {
        comp = c_instruction;
	}
    else if(it1 != std::string::npos && it2 == std::string::npos) {
        comp = c_instruction.substr(it1 + 1);
	}
    else if(it1 == std::string::npos && it2 != std::string::npos) {
        comp = c_instruction.substr(0, it2);
	}
    else if(it1 != std::string::npos && it2 != std::string::npos) {
        comp = c_instruction.substr(it1 + 1, it2 - it1 - 1);
	}
	
	if(!comp.empty() && !comp_encodings.count(comp)) {
		throw InvalidCompErr(c_instruction);
	}

	// attempt to extract jmp part
    it = c_instruction.find(';');
    if(it != std::string::npos) {
		jmp = c_instruction.substr(it + 1);
		if(!jmp_encodings.count(jmp)) {
			throw InvalidJmpErr(c_instruction);
		}
	}
}

uint16_t CInstruction::encode() const {
	uint16_t machine_code = (7<<13); //Every Hack assembly instruction begins with 111
	
	machine_code |= comp_encodings[comp] << 6; // followed by 7-bit comp part starting at the 4th bit
	machine_code |= dst_encodings[dst] << 3;	// followed by 3-bit dst part starting at the 11th bit 
	machine_code |= jmp_encodings[jmp];	// followed by 3-bit jmp part starting at the 14th bit

	return machine_code;
}

const InstructionType& CInstruction::type() const {
	static constexpr InstructionType type = InstructionType::C;
	return type;
}

AInstruction::AInstruction(const std::string& a_instruction) {
	/*
		An a-instruction must begin with '@' and must be at least two characters wide.

		If what follows after '@' can be converted into integer, make sure that it
		is not greater than 32768.
	*/
	if(a_instruction.empty()) {
		throw std::runtime_error("empty instruction");
	}

	if(a_instruction.front() != '@' || a_instruction.size() == 1) {
		throw std::runtime_error(std::string("invalid instruction: ") + a_instruction);
	}

	value = a_instruction.substr(1);
	if(is_constant()) {
		try {
			auto n = std::stoi(value);
			if(n > 32768) {	// constant value cannot be larger than 2^15
				throw std::runtime_error(std::string("invalid instruction: ") + a_instruction);
			}
		}
		catch(const std::exception& ex) {
			throw std::runtime_error(std::string("invalid instruction: ") + a_instruction);
		}
	}
}

bool AInstruction::is_constant() const {
    return std::all_of(value.begin(), value.end(), ::isdigit);
}

uint16_t AInstruction::encode(const SymbolTable& symbol_table) const {
    if(is_constant())
	    return static_cast<uint16_t>(std::stoi(value));
    else
        return symbol_table.get(value);
}

const std::string& AInstruction::peek_value() const { return value; }

const InstructionType& AInstruction::type() const {
	static constexpr InstructionType type = InstructionType::A;
	return type;
}

Label::Label(const std::string& label) {
	/*
		A label must begin with '(' and end with ')', and must have at least one character in between.
	*/
	if(label.empty()) {
		throw std::runtime_error("empty instruction");
	}

	if(label.front() != '(' || label.back() != ')' || label.size() < 3) {
		throw std::runtime_error(std::string("invalid instruction: ") + label);
	}

	value = label.substr(1, label.size() - 2);
}

const InstructionType& Label::type() const {
	static constexpr InstructionType type = InstructionType::LABEL;
	return type;
}

const std::string& Label::peek_value() const { return value; }

std::unique_ptr<Instruction> InstructionFactory::create_instance(const std::string& instruction) {
	if(instruction.front() == '@') {
		return std::make_unique<AInstruction>(instruction);
	}
	else if(instruction.front() == '(') {
		return std::make_unique<Label>(instruction);
	}
	else {
		return std::make_unique<CInstruction>(instruction);
	}
}
