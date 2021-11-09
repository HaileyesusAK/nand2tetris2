#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__
#include <algorithm>
#include <string>

class Assembler {
    public:
        std::string translate(const std::string& asm_instruction) {
            if(is_number(asm_instruction.substr(1)))
                return "0000000000000000";
            else
                return "0000000000010000";
        }

    private:
        bool is_number(const std::string s) {
            return !s.empty() and std::all_of(s.begin(), s.end(), ::isdigit);
        }
};

#endif
