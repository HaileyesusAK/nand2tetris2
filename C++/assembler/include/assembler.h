#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__
#include <algorithm>
#include <bitset>
#include <string>
#include <unordered_map>

static const size_t WSIZE = 16;

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
            return std::bitset<WSIZE>(machine_code).to_string();
        }

        std::string encode_comp_instruction(const std::string& instruction) const {
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

			return get_binary_instruction(it->second << OFFSET);
		}

        std::string encode_dst_instruction(const std::string& instruction) const {
            std::unordered_map<std::string, uint16_t> encodings {
                {"M", 1}, {"D", 2}, {"MD", 3}, {"A", 4}, {"AM", 5},
				{"AD", 6}, {"AMD", 7}
            };

			const size_t OFFSET = 3;
			auto it = encodings.find(instruction);
			if(it == encodings.end())
				return "";

			return get_binary_instruction(it->second << OFFSET);
		}

		void split_c_instruction(const std::string& instruction, std::string& comp,
								 std::string& dst) const {
            auto n = instruction.find('=');
			comp = "";
			dst = "";

            if(n == std::string::npos) {
				comp = instruction;
            }
            else {
				dst = instruction.substr(0, n);
				comp = instruction.substr(n+1);
            }
		}

        std::string encode_c_instruction(const std::string& c_instruction) const {
			std::string comp, dst;
			const size_t OFFSET = 13;
			std::string prefix {std::bitset<WSIZE>(7 << OFFSET).to_string()};

			split_c_instruction(c_instruction, comp, dst);
			comp = encode_comp_instruction(comp);

			auto machine_code = std::bitset<WSIZE> {prefix} | std::bitset<WSIZE> {comp};

			if(!dst.empty())
				machine_code |= std::bitset<WSIZE> {encode_dst_instruction(dst)};

			return machine_code.to_string();
        }
};

#endif
