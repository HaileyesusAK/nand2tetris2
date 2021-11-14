#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

#include <string>
#include <unordered_map>

static const size_t WSIZE = 16;
static const uint16_t VARIABLE_START_ADDRESS = 16;

using CodeMap = std::unordered_map<std::string, uint16_t>;

class Assembler {
    public:
        Assembler();

        bool add_symbol(const std::string& name, uint16_t address);
        bool add_variable(const std::string& name);
        void reset_symbol_table();
        std::string translate(const std::string& asm_instruction);

    private:
        static inline uint16_t next_symbol_addr;
        static inline CodeMap symbol_table;
		static inline const CodeMap predefined_symbols {
			{"SP", 0}, {"LCL", 1}, {"ARG", 2}, {"THIS", 3},
			{"THAT", 4}, {"SCREEN", 16384}, {"KBD", 24576}
		};
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
