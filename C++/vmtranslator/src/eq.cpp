#include <fstream>
#include <iostream>
#include "eq.h"

namespace vm_command {

    void Eq::to_asm(std::ofstream &out) const {
        static int label_counter = 0;
        write_asm(out, "// eq", "");
        prepend(out);
        std::string label {std::string("EQ_END_") + std::to_string(label_counter++)};
        write_asm(out, "@" + label);
        write_asm(out, "D;JEQ");
        append(out);
        write_asm(out, "(" + label + ")");
    }

}
