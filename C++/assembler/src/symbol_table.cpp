#include <stdexcept>
#include <string>
#include <unordered_map>
#include "symbol_table.h"


uint16_t SymbolTable::insert(const std::string& label, uint16_t address) {
	auto ret = table.emplace(label, address);
	return ret.first->second;
}

uint16_t SymbolTable::insert(const std::string& variable) {
	auto ret = table.emplace(variable, variable_address);
	if(ret.second)
		variable_address++;

	return ret.first->second;
}

uint16_t SymbolTable::get(const std::string& symbol) const {
	try {
		return table.at(symbol);
	}
	catch(const std::exception& ex) {
		throw std::runtime_error(std::string("unknown symbol: " + symbol));
	}
}
