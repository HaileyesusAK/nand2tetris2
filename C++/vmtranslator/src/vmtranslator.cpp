#include <string>
#include <unordered_map>
#include <vector>
#include "vmtranslator.h"

std::vector<std::string> VmTranslator::translate(const BinaryAluOp& op) {
    std::vector<std::string> inst {"@SP", "AM=M-1", "D=M", "@SP", "A=M-1"};
    
    switch(op) {
        case BinaryAluOp::ADD: inst.push_back("M=M+D"); break;
        case BinaryAluOp::SUB: inst.push_back("M=M-D"); break;
        case BinaryAluOp::AND: inst.push_back("M=D&M"); break;
        case BinaryAluOp::OR:  inst.push_back("M=D|M"); break;
    }

    return inst;
}
