#include <filesystem>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>
#include "vmtranslator.h"

InstList VmTranslator::translate(const BinaryAluOp& op) {
    InstList inst {"@SP", "AM=M-1", "D=M", "@SP", "A=M-1"};

    switch(op) {
        case BinaryAluOp::ADD: inst.push_back("M=M+D"); break;
        case BinaryAluOp::SUB: inst.push_back("M=M-D"); break;
        case BinaryAluOp::AND: inst.push_back("M=D&M"); break;
        case BinaryAluOp::OR:  inst.push_back("M=D|M"); break;
    }

    return inst;
}

InstList VmTranslator::translate(const RelOp& op, uint16_t pc) {
    InstList inst { "@SP", "AM=M-1", "D=M", "@SP", "A=M-1", "D=M-D"};
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

InstList VmTranslator::translate(const UnaryOp& op) {
    InstList inst { "@SP", "A=M-1" };
    if(op == UnaryOp::NEG)
        inst.push_back("M=-M");
    else
        inst.push_back("M=!M");

    return inst;
}


InstList VmTranslator::translate_and() {
    return translate(BinaryAluOp::AND);
}

InstList VmTranslator::translate_or() {
    return translate(BinaryAluOp::OR);
}

InstList VmTranslator::translate_add() {
    return translate(BinaryAluOp::ADD);
}

InstList VmTranslator::translate_sub() {
    return translate(BinaryAluOp::SUB);
}

InstList VmTranslator::translate_eq() {
    return translate(RelOp::EQ, program_counter); 
}

InstList VmTranslator::translate_lt() {
    return translate(RelOp::LT, program_counter); 
}

InstList VmTranslator::translate_gt() {
    return translate(RelOp::GT, program_counter); 
}

InstList VmTranslator::translate_not() {
    return translate(UnaryOp::NOT);
}

InstList VmTranslator::translate_neg() {
    return translate(UnaryOp::NEG);
}

void VmTranslator::append_push_D(InstList& instructions) {
    const static InstList push_D_instructions {
        "@SP", "A=M", "M=D", "@SP", "M=M+1"
    };

    for(const auto& inst : push_D_instructions)
        instructions.push_back(inst);
}

void VmTranslator::append_pop_D(InstList& instructions) {
    const static InstList push_D_instructions {
        "@SP", "AM=M-1", "D=M"
    };

    for(const auto& inst : push_D_instructions)
        instructions.push_back(inst);
}

InstList VmTranslator::translate_push(const Segment& segment, uint16_t idx) {
    const static std::unordered_map<Segment, std::string> segments {
        {Segment::ARG, "@ARG"},
        {Segment::LCL, "@LCL"},
        {Segment::THIS, "@THIS"},
        {Segment::THAT, "@THAT"},
        {Segment::POINTER, "@R3"},
        {Segment::TEMP, "@R5"}
    };

    InstList instructions {
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

InstList VmTranslator::translate_pop(const Segment& segment, uint16_t idx) {
    const static std::unordered_map<Segment, std::string> segments {
        {Segment::ARG, "@ARG"},
        {Segment::LCL, "@LCL"},
        {Segment::THIS, "@THIS"},
        {Segment::THAT, "@THAT"},
        {Segment::POINTER, "@R3"},
        {Segment::TEMP, "@R5"}
    };

    // Move the segment's base pointer to the address of the ith element
    InstList instructions {
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

InstList VmTranslator::translate_push_static(const std::string& file_name,  uint16_t idx) {
    InstList instructions {
        "@" + file_name + "." + std::to_string(idx),
        "D=M"
    };
    append_push_D(instructions);

    return instructions;
}

InstList VmTranslator::translate_pop_static(const std::string& file_name,  uint16_t idx) {
    InstList instructions;

    append_pop_D(instructions);
    instructions.push_back("@" + file_name + "." + std::to_string(idx));
    instructions.push_back("M=D");

    return instructions;
}

InstList VmTranslator::translate_push_constant(uint16_t idx) {
    InstList instructions { "@" + std::to_string(idx), "D=A" };
    append_push_D(instructions);

    return instructions;
}

InstList VmTranslator::split_command(const std::string& vm_cmd) {
    static std::string segments {"argument|local|this|that|temp|pointer|static"};
    static std::string push_pattern {R"((push)\s+(constant|)" + segments + R"()\s+(\d+))"};
    static std::string pop_pattern {R"((pop)\s+()" + segments + R"()\s+(\d+))"};
    static std::string alu_pattern {"(add|sub|and|or|lt|eq|gt|not|neg)"};
    static std::regex cmd_regex {push_pattern + "|" + pop_pattern + "|" + alu_pattern};

    InstList cmd_parts;

    std::smatch matches;
    if(std::regex_match(vm_cmd, matches, cmd_regex))
        for(size_t i = 1; i < matches.size(); ++i)
            if(matches[i].matched)
                cmd_parts.push_back(matches[i].str());

    return cmd_parts;
}

void VmTranslator::translate(const fs::path& vm_file_path) {
    if(!fs::exists(vm_file_path))
        throw std::runtime_error("Input file doesn't exist");

    std::unordered_map<std::string, InstList (VmTranslator::*)()> alu_translators {
        {"add", &VmTranslator::translate_add},
        {"sub", &VmTranslator::translate_sub},
        {"and", &VmTranslator::translate_and},
        {"or", &VmTranslator::translate_or},
        {"eq", &VmTranslator::translate_eq},
        {"lt", &VmTranslator::translate_lt},
        {"gt", &VmTranslator::translate_gt},
        {"not", &VmTranslator::translate_not},
        {"neg", &VmTranslator::translate_neg}
    };

    auto asm_file_path = vm_file_path;
    asm_file_path.replace_extension(".asm");

    std::ifstream ifs {vm_file_path};
    std::ofstream ofs {asm_file_path};

    std::string line;
    while(!ifs.eof()) {
        getline(ifs, line);

        InstList vm_cmd = split_command(line);
        if(vm_cmd.size() == 1) {
            auto translator = alu_translators.at(vm_cmd[0]);
            auto instructions = (this->*translator)();
            std::copy(instructions.begin(),
                      instructions.end(),
                      std::ostream_iterator<std::string>(ofs, "\n"));

            program_counter += instructions.size();
        }
    }
}
