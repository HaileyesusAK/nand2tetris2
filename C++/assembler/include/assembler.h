#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__
#include <algorithm>
#include <bitset>
#include <string>
#include <unordered_map>


class Assembler {
    public:
        std::string translate(const std::string& asm_instruction) {
            if(asm_instruction.front() == '@') {
                if(is_number(asm_instruction.substr(1)))
                    return "0000000000000000";
                else
                    return "0000000000010000";
            }
            else
                return encode_c_instruction(asm_instruction);
        }

    private:
        bool is_number(const std::string s) const {
            return !s.empty() and std::all_of(s.begin(), s.end(), ::isdigit);
        }

        std::string get_binary_instruction(uint16_t machine_code) const {
            return std::bitset<16>(machine_code).to_string();
        }

        std::string encode_c_instruction(const std::string& c_instruction) const {
            std::unordered_map<std::string, uint16_t> encodings {
                {"0", 42}, {"1", 63}, {"-1", 58}, {"D", 12}, {"A", 48},
                {"!D", 13}, {"!A", 49}, {"-D", 15}, {"-A", 51}, {"D+1", 31},
                {"A+1", 55}, {"D-1", 14}, {"A-1", 50}, {"D+A", 2}, {"D-A", 19},
                {"A-D", 7}, {"D&A", 0}, {"D|A", 21}, {"M", 112}, {"!M", 113},
                {"-M", 115}, {"M+1", 119}, {"M-1", 114}, {"D+M", 66},
                {"D-M", 83}, {"M-D", 71}, {"D&M", 64}, {"D|M", 85}
            };

            auto machine_code = 7 << 13;
            machine_code |= (encodings.find(c_instruction)->second) << 6;
            return get_binary_instruction(machine_code);
        }
};

#endif
