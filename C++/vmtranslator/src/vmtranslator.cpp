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

std::vector<std::string> VmTranslator::translate(const RelOp& op, uint16_t pc) {
    std::vector<std::string> inst { "@SP", "AM=M-1", "D=M", "@SP", "A=M-1", "D=M-D"};
    inst.push_back("M=-1");     //Assume the arguments are equal

    // the next instruction address to execute immediately if the arguments are not equal
    pc += inst.size() + 3;
    inst.push_back("@" + std::to_string(pc));

    switch(op) {
        case RelOp::EQ: inst.push_back("D;JEQ"); break;
        case RelOp::GT: inst.push_back("D;JGT"); break;
        case RelOp::LT: inst.push_back("D;JLT"); break;
    }

    inst.push_back("M=0");

    return inst;
}

std::vector<std::string> VmTranslator::translate(const UnaryOp& op) {
    std::vector<std::string> inst { "@SP", "A=M-1" };
    if(op == UnaryOp::NEG)
        inst.push_back("M=-M");
    else
        inst.push_back("M=!M");

    return inst;
}
