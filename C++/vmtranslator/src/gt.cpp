#include <fstream>
#include <iostream>
#include "gt.h"

namespace vm_command {

    void Gt::to_asm(std::ofstream &out) const {
        static int label_counter = 0;
        write_asm(out, "// gt", "");
        prepend(out);
        std::string label {std::string("GT_END_") + std::to_string(label_counter++)};
        write_asm(out, "@" + label);
        write_asm(out, "D;JGT");
        append(out);
        write_asm(out, "(" + label + ")");
    }

}
