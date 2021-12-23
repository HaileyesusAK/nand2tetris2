#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

/*
	Defines symbol table used when translating an hack assembly
	instructions into their respective machine instructions.
*/

#include <string>
#include <unordered_map>

class SymbolTable {
	private:
		std::unordered_map<std::string, uint16_t> table {
			//Always starts with the predefined symbols
			{"SP", 0}, {"LCL", 1}, {"ARG", 2}, {"THIS", 3}, {"THAT", 4}, {"R0", 0},
			{"R1", 1}, {"R2", 2}, {"R3", 3}, {"R4", 4}, {"R5", 5}, {"R6", 6}, {"R7", 7},
			{"R8", 8}, {"R9", 9}, {"R10", 10}, {"R11", 11}, {"R12", 12}, {"R13", 13},
			{"R14", 14}, {"R15", 15}, {"SCREEN", 16384}, {"KBD", 24576}
		};

		uint16_t variable_address = 16;		// Variables start at address 16

	public:
		/*
			Attempts to insert a new variable and return the associated address.
			If the variable exists, return its address.
		*/
		uint16_t insert(const std::string& variable);

		/*
			Attempts to insert a new symbol with associated with the given address.
			If the symbol exists, return the previously associated address.
		*/
		uint16_t insert(const std::string& label, uint16_t address);

		/*
			Attempts to query the address associated with the given symbol.
			If the symbol doesn't exist, raises a std::runtime_error
		*/
		uint16_t get(const std::string& symbol) const;
};

#endif
