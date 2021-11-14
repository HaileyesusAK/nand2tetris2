#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

#include <string>
#include <unordered_map>

static const size_t WSIZE = 16;
static const uint16_t VARIABLE_START_ADDRESS = 16;

class Assembler {
    public:
        Assembler();

        bool add_symbol(const std::string& name, uint16_t address);
        bool add_variable(const std::string& name);
        void reset_symbol_table();
        std::string translate(const std::string& asm_instruction);

    private:
        static inline uint16_t next_symbol_addr;
        static inline std::unordered_map<std::string, uint16_t> symbol_table;

        std::string compact_instruction(const std::string& instruction) const;
        uint16_t get_address(const std::string& symbol) const;
        std::string get_binary_string(uint16_t machine_code) const;
        bool is_number(const std::string s) const;
        std::string encode_c_instruction(const std::string& c_instruction) const;
        std::string encode_comp_instruction(const std::string& instruction) const;
        std::string encode_dst_instruction(const std::string& instruction) const;
        std::string encode_jmp_instruction(const std::string& instruction) const;
        std::string extract_comp(const std::string& c_instruction) const;
        std::string extract_dst(const std::string& c_instruction) const;
        std::string extract_jmp(const std::string& c_instruction) const;
};

#endif
