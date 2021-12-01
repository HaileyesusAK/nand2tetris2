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

void VmTranslator::append_pop_D(std::vector<std::string>& instructions) {
    const static std::vector<std::string> push_D_instructions {
        "@SP", "AM=M-1", "D=M"
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

std::vector<std::string> VmTranslator::translate_pop(const Segment& segment, uint16_t idx) {
    const static std::unordered_map<Segment, std::string> segments {
        {Segment::ARG, "@ARG"},
        {Segment::LCL, "@LCL"},
        {Segment::THIS, "@THIS"},
        {Segment::THAT, "@THAT"},
        {Segment::POINTER, "@R3"},
        {Segment::TEMP, "@R5"}
    };

    // Move the segment's base pointer to the address of the ith element
    std::vector<std::string> instructions {
        "@" + std::to_string(idx),
        "D=A",
        segments.at(segment),
        "M=D+M"
    };

    // Push D onto to the ith element
    append_pop_D(instructions);
    instructions.push_back(segments.at(segment));
    instructions.push_back("A=M");
    instructions.push_back("M=D");

    // Move the segment's base pointer to the start of the segment
    instructions.push_back("@" + std::to_string(idx));
    instructions.push_back("D=A");
    instructions.push_back(segments.at(segment));
    instructions.push_back("M=M-D");

    return instructions;
}

std::vector<std::string> VmTranslator::translate_push_static(const std::string& file_name,  uint16_t idx) {
    std::vector<std::string> instructions {
        "@" + file_name + "." + std::to_string(idx),
        "D=M"
    };
    append_push_D(instructions);

    return instructions;
}

std::vector<std::string> VmTranslator::translate_push_constant(uint16_t idx) {
    std::vector<std::string> instructions { "@" + std::to_string(idx), "D=A" };
    append_push_D(instructions);

    return instructions;
}
