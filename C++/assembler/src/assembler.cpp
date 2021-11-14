#include <algorithm>
#include <bitset>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <string>
#include <unordered_map>

#include "assembler.h"

Assembler::Assembler() {
    reset_symbol_table();
}

bool Assembler::add_symbol(const std::string& name, uint16_t address) {
    return  symbol_table.insert({name, address}).second;
}

bool Assembler::add_variable(const std::string& name) {
    bool inserted = symbol_table.insert({name, next_symbol_addr}).second;
    if(inserted)
        ++next_symbol_addr;

    return inserted;
}

void Assembler::build_symbol_table(const fs::path& asm_file_path) {
    if(!fs::exists(asm_file_path))
        return;

    std::string line;
    std::unordered_map<std::string, uint16_t> variables;
    uint16_t pc = 0;
    
    std::ifstream ifs {asm_file_path};
    while(!ifs.eof()) {
        std::getline(ifs, line);
        if(!line.empty())
            line.pop_back(); // remove trailing newline 

        auto instruction = classify_instruction(line);

        if(instruction.second == InstType::A) {
            if(!is_number(instruction.first))
                variables.insert({instruction.first, pc}); // assume a symbol is a variable
            ++pc;
        }
        else if(instruction.second == InstType::LABEL) {
            variables.erase(instruction.first); // since the symbol is a label
            add_symbol(instruction.first, pc);
        }
        else if(instruction.second == InstType::C)
            ++pc;
    }

    // The variables must be added in the order they are encountered
    auto flip = [](const std::pair<std::string, uint16_t>& e){
        return std::pair<uint16_t, std::string>{e.second, e.first}; 
    };
    std::map<uint16_t, std::string> sorted_variables;
    std::transform(variables.begin(),
                   variables.end(),
                   std::inserter(sorted_variables, sorted_variables.begin()),
                   flip);

    for(const auto& variable : sorted_variables)
        add_variable(variable.second);
}

std::pair<std::string, InstType> Assembler::classify_instruction(const std::string& inst) const {
	auto s = compact_instruction(inst);
	if(s.empty())
		return {"", InstType::BLANK};
	else if(s.front() == '@' && s.length() > 1)
		return {s.substr(1), InstType::A};
	else if(s.front() == '(' && s.back() == ')' && s.length() > 2)
		return {s.substr(1, s.length() - 2), InstType::LABEL};
	else {
		std::string dst = extract_dst(s);
		std::string comp = extract_comp(s);
		std::string jmp = extract_jmp(s);

		if(!dst.empty() && !dst_encodings.count(dst))
			return {inst, InstType::UNKNOWN};

		if(!jmp.empty() && !jmp_encodings.count(jmp))
			return {inst, InstType::UNKNOWN};

		if(!comp.empty() && !comp_encodings.count(comp))
			return {inst, InstType::UNKNOWN};

		return {s, InstType::C};
	}
}

std::string Assembler::compact_instruction(const std::string& instruction) const {
    std::string s = instruction.substr(0, instruction.find("//"));
    auto end = std::remove(s.begin(), s.end(), ' ');
    s.erase(end, s.end());
    return s;
}

std::string Assembler::encode_c_instruction(const std::string& c_instruction) const {
    const size_t OFFSET = 13;
    std::string prefix {std::bitset<WSIZE>(7 << OFFSET).to_string()};

    std::string dst = extract_dst(c_instruction);
    std::string comp = extract_comp(c_instruction);
    std::string jmp = extract_jmp(c_instruction);
    comp = encode_comp_instruction(comp);

    auto machine_code = std::bitset<WSIZE> {prefix} | std::bitset<WSIZE> {comp};

    if(!dst.empty())
        machine_code |= std::bitset<WSIZE> {encode_dst_instruction(dst)};

    if(!jmp.empty())
        machine_code |= std::bitset<WSIZE> {encode_jmp_instruction(jmp)};

    return machine_code.to_string();
}

std::string Assembler::encode_comp_instruction(const std::string& instruction) const {
    const size_t OFFSET = 6;
    auto it = comp_encodings.find(instruction);
    if(it == comp_encodings.end())
        return "";

    return get_binary_string(it->second << OFFSET);
}

std::string Assembler::encode_dst_instruction(const std::string& instruction) const {
    const size_t OFFSET = 3;
    auto it = dst_encodings.find(instruction);
    if(it == dst_encodings.end())
        return "";

    return get_binary_string(it->second << OFFSET);
}

std::string Assembler::encode_jmp_instruction(const std::string& instruction) const {
    auto it = jmp_encodings.find(instruction);
    if(it == jmp_encodings.end())
        return "";

    return get_binary_string(it->second);
}

std::string Assembler::extract_comp(const std::string& c_instruction) const {
    auto it1 = c_instruction.find('=');
    auto it2 = c_instruction.find(';');

    if(it1 == std::string::npos && it2 == std::string::npos)
        return c_instruction;
    else if(it1 != std::string::npos && it2 == std::string::npos)
        return c_instruction.substr(it1 + 1);
    else if(it1 == std::string::npos && it2 != std::string::npos)
        return c_instruction.substr(0, it2);
    else if(it1 != std::string::npos && it2 != std::string::npos)
        return c_instruction.substr(it1 + 1, it2 - it1 - 1);
    else
        return "";
}

std::string Assembler::extract_dst(const std::string& c_instruction) const {
    auto it = c_instruction.find('=');
    if(it == std::string::npos)
        return "";

    return c_instruction.substr(0, it);
}

std::string Assembler::extract_jmp(const std::string& c_instruction) const {
    auto it = c_instruction.find(';');
    if(it == std::string::npos)
        return "";

    return c_instruction.substr(it + 1);
}

std::string Assembler::get_binary_string(uint16_t machine_code) const {
    return std::bitset<WSIZE>(machine_code).to_string();
}

uint16_t Assembler::get_address(const std::string& symbol) const {
    auto it = symbol_table.find(symbol);
    if(it == symbol_table.end())
        return 0;

    return it->second;
}

bool Assembler::is_number(const std::string s) const {
    return !s.empty() and std::all_of(s.begin(), s.end(), ::isdigit);
}

void Assembler::reset_symbol_table() {
    symbol_table = predefined_symbols;
    next_symbol_addr = VARIABLE_START_ADDRESS;
}

std::string Assembler::translate(const std::pair<std::string, InstType>& instruction) const {
	switch(instruction.second) {
		case InstType::A:
			if(is_number(instruction.first))
				return get_binary_string(static_cast<uint16_t>(std::stoul(instruction.first)));
			else
				return get_binary_string(get_address(instruction.first));
		break;

		case InstType::C:
			return encode_c_instruction(instruction.first);
		break;

		default:
			return "";
	}
}

std::string Assembler::translate(const std::string& asm_instruction) {
	auto instruction = classify_instruction(asm_instruction);
	return translate(instruction);
}


void Assembler::translate_file(const fs::path& asm_file_path) {
    if(!fs::exists(asm_file_path))
        return;

    reset_symbol_table();

    // create output file
    auto output_path = asm_file_path;
    output_path.replace_extension(".hack");
    std::ofstream ofs {output_path};

    // create symbol table
    build_symbol_table(asm_file_path);

    // translate instructions from the input file
    std::string line;
    std::ifstream ifs {asm_file_path};
    while(!ifs.eof()) {
        std::getline(ifs, line);
        if(!line.empty())
            line.pop_back(); // remove trailing newline

        auto instruction = classify_instruction(line);
        if(instruction.second == InstType::A || instruction.second == InstType::C) {
            ofs << translate(instruction) << std::endl;
        }
    }
}
