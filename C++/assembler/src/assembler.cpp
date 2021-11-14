#include <algorithm>
#include <bitset>
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
    std::unordered_map<std::string, uint16_t> encodings {
        {"0", 42}, {"1", 63}, {"-1", 58}, {"D", 12}, {"A", 48},
        {"!D", 13}, {"!A", 49}, {"-D", 15}, {"-A", 51}, {"D+1", 31},
        {"A+1", 55}, {"D-1", 14}, {"A-1", 50}, {"D+A", 2}, {"D-A", 19},
        {"A-D", 7}, {"D&A", 0}, {"D|A", 21}, {"M", 112}, {"!M", 113},
        {"-M", 115}, {"M+1", 119}, {"M-1", 114}, {"D+M", 66},
        {"D-M", 83}, {"M-D", 71}, {"D&M", 64}, {"D|M", 85}
    };

    const size_t OFFSET = 6;
    auto it = encodings.find(instruction);
    if(it == encodings.end())
        return "";

    return get_binary_string(it->second << OFFSET);
}

std::string Assembler::encode_dst_instruction(const std::string& instruction) const {
    std::unordered_map<std::string, uint16_t> encodings {
        {"M", 1}, {"D", 2}, {"MD", 3}, {"A", 4}, {"AM", 5},
        {"AD", 6}, {"AMD", 7}
    };

    const size_t OFFSET = 3;
    auto it = encodings.find(instruction);
    if(it == encodings.end())
        return "";

    return get_binary_string(it->second << OFFSET);
}

std::string Assembler::encode_jmp_instruction(const std::string& instruction) const {
    std::unordered_map<std::string, uint16_t> encodings {
        {"JGT", 1}, {"JEQ", 2}, {"JGE", 3}, {"JLT", 4}, {"JNE", 5},
        {"JLE", 6}, {"JMP", 7}
    };

    auto it = encodings.find(instruction);
    if(it == encodings.end())
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
    static std::unordered_map<std::string, uint16_t> predefined_symbols {
        // initialize with addresses of predefined symbol
        {"SP", 0}, {"LCL", 1}, {"ARG", 2}, {"THIS", 3},
        {"THAT", 4}, {"SCREEN", 16384}, {"KBD", 24576}
    };

    symbol_table = predefined_symbols;
    next_symbol_addr = VARIABLE_START_ADDRESS;
}

std::string Assembler::translate(const std::string& asm_instruction) {
    auto instruction = compact_instruction(asm_instruction);
    if(instruction.empty())
        return "";

    if(instruction.front() == '@') {
        auto symbol = instruction.substr(1);
        if(is_number(symbol))
            return get_binary_string(static_cast<uint16_t>(std::stoul(symbol)));
        else
            return get_binary_string(get_address(symbol));
    }
    else
        return encode_c_instruction(instruction);
}
