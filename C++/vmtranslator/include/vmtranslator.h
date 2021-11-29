#ifndef __VM_TRANSLATOR_H__
#define __VM_TRANSLATOR_H__

#include<string>
#include<vector>

enum class BinaryAluOp {ADD, SUB, AND, OR};
enum class RelOp {EQ, GT, LT};

class VmTranslator {
    public:
        std::vector<std::string> translate(const BinaryAluOp& op);
        std::vector<std::string> translate(const RelOp& op, uint16_t pc);
};

#endif
