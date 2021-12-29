#include <fstream>
#include <iostream>
#include "lt.h"

namespace vm_command {

    void Lt::to_asm(std::ofstream &out) const {
        static int label_counter = 0;
        write_asm(out, "// lt", "");
        prepend(out);
        std::string label {std::string("LT_END_") + std::to_string(label_counter++)};
        write_asm(out, "@" + label);
        write_asm(out, "D;JLT");
        append(out);
        write_asm(out, "(" + label + ")");
    }

}
