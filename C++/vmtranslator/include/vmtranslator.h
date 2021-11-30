#ifndef __VM_TRANSLATOR_H__
#define __VM_TRANSLATOR_H__

#include<string>
#include<vector>

enum class BinaryAluOp {ADD, SUB, AND, OR};
enum class RelOp {EQ, GT, LT};
enum class UnaryOp {NEG, NOT};
enum class Segment {ARG, LCL, THIS, THAT, POINTER, TEMP};

class VmTranslator {
    public:
        std::vector<std::string> translate(const BinaryAluOp& op);
        std::vector<std::string> translate(const RelOp& op, uint16_t pc);
        std::vector<std::string> translate(const UnaryOp& op);
        std::vector<std::string> translate_push(const Segment& segment, uint16_t idx);
        std::vector<std::string> translate_push_static(const std::string& file_name,  uint16_t idx);
        std::vector<std::string> translate_push_constant(uint16_t idx);

    private:
        void append_push_D(std::vector<std::string>& instructions);
};

#endif
