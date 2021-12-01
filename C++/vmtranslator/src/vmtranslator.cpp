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
    pc += inst.size() + 5;
    inst.push_back("@" + std::to_string(pc));

    switch(op) {
        case RelOp::EQ: inst.push_back("D;JEQ"); break;
        case RelOp::GT: inst.push_back("D;JGT"); break;
        case RelOp::LT: inst.push_back("D;JLT"); break;
    }

    inst.push_back("@SP");
    inst.push_back("A=M-1");
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

void VmTranslator::append_push_D(std::vector<std::string>& instructions) {
    const static std::vector<std::string> push_D_instructions {
        "@SP", "A=M", "M=D", "@SP", "M=M+1"
    };

    for(const auto& inst : push_D_instructions)
        instructions.push_back(inst);
}

std::vector<std::string> VmTranslator::translate_push(const Segment& segment, uint16_t idx) {
    const static std::unordered_map<Segment, std::string> segments {
        {Segment::ARG, "@ARG"},
        {Segment::LCL, "@LCL"},
        {Segment::THIS, "@THIS"},
        {Segment::THAT, "@THAT"},
        {Segment::POINTER, "@R3"},
        {Segment::TEMP, "@R5"}
    };

    std::vector<std::string> instructions {
        // Put the ith element from the segment in D
        segments.at(segment),
        "D=M",
        "@" + std::to_string(idx),
        "A=D+A",
        "D=M"
    };
    append_push_D(instructions);

    return instructions;
}

std::vector<std::string> VmTranslator::translate_push_static(const std::string& file_name,  uint16_t idx) {
    std::vector<std::string> instructions {
        "@" + file_name + "." + std::to_string(idx),
        "D=A"
    };
    append_push_D(instructions);

    return instructions;
}

std::vector<std::string> VmTranslator::translate_push_constant(uint16_t idx) {
    std::vector<std::string> instructions { "@" + std::to_string(idx), "D=A" };
    append_push_D(instructions);

    return instructions;
}
