#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

/*
	Defines classes to represent the various Hack assembly instructions.

*/

#include <bitset>
#include <memory>
#include <string>
#include <unordered_map>
#include "symbol_table.h"

enum class InstructionType { A, C, LABEL, UNKNOWN };

class InvalidDstErr : public std::runtime_error {
	public:
		InvalidDstErr(const std::string& s)
			: std::runtime_error(std::string("invalid destination part: ") + s) {} 
};

class InvalidCompErr : public std::runtime_error {
	public:
		InvalidCompErr(const std::string& s)
			: std::runtime_error(std::string("invalid comp part: ") + s) {} 
};

class InvalidJmpErr : public std::runtime_error {
	public:
		InvalidJmpErr(const std::string& s)
			: std::runtime_error(std::string("invalid jmp part: ") + s) {} 
};

/* Represents a generic Hack assembly instruction */
class Instruction {
	private:
		std::string value;

	public:
		virtual const InstructionType& type() const;

		virtual ~Instruction() = default;

		/*
			Return integral representation of Hack machine code so that is
			when the returned integer is converted into a 16-bit string,
			it represents a valid Hack machine code.
		*/
		virtual uint16_t encode() const { return 0; }

		virtual const std::string& peek_value() const;

		/* removes whitespaces and comments from the instruction */
		static std::string compact(const std::string& instruction);
};

/* Represents Hack assembly c-instruction */
class CInstruction : public Instruction {
	private:
		std::string dst;
		std::string comp;
		std::string jmp;

		using CodeMap = std::unordered_map<std::string, uint16_t>;

		static inline CodeMap comp_encodings {
			{"0", 42}, {"1", 63}, {"-1", 58}, {"D", 12}, {"A", 48}, {"!D", 13},
			{"!A", 49}, {"-D", 15}, {"-A", 51}, {"D+1", 31}, {"A+1", 55},
			{"D-1", 14}, {"A-1", 50}, {"D+A", 2}, {"D-A", 19}, {"A-D", 7},
			{"D&A", 0}, {"D|A", 21}, {"M", 112}, {"!M", 113}, {"-M", 115},
			{"M+1", 119}, {"M-1", 114}, {"D+M", 66}, {"D-M", 83}, {"M-D", 71},
			{"D&M", 64}, {"D|M", 85}
		};

		static inline CodeMap dst_encodings {
			{"M", 1}, {"D", 2}, {"MD", 3}, {"A", 4}, {"AM", 5},
			{"AD", 6}, {"AMD", 7}
		};

		static inline CodeMap jmp_encodings {
			{"JGT", 1}, {"JEQ", 2}, {"JGE", 3}, {"JLT", 4}, {"JNE", 5},
			{"JLE", 6}, {"JMP", 7}
		};

	public:
		const InstructionType& type() const;

		CInstruction(const std::string& instruction);

		uint16_t encode() const;
};

/* Represents Hack assembly A-instruction */
class AInstruction : public Instruction {
	private:
		std::string value;
	
	public:
		AInstruction(const std::string& a_instruction);

		bool is_constant() const;

		const InstructionType& type() const;

		const std::string& peek_value() const;
	
		uint16_t encode(const SymbolTable& symbol_table) const;
};

/* Represents Hack assembly Label */
class Label : public Instruction {
	private:
		std::string value;
	
	public:
		const InstructionType& type() const;

		const std::string& peek_value() const;

		Label(const std::string& a_instruction);
};

class InstructionFactory {
	public:
		static std::unique_ptr<Instruction> create_instance(const std::string&);
};

#endif
