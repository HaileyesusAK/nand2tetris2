#ifndef __VM_TRANSLATOR_H__
#define __VM_TRANSLATOR_H__

#include<string>
#include<vector>

enum class BinaryAluOp {ADD, SUB, AND, OR};

class VmTranslator {
    public:
        std::vector<std::string> translate(const BinaryAluOp& op);
};

#endif
