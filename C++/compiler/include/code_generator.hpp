#ifndef __CODE_GENERATOR_H__
#define __CODE_GENERATOR_H__

#include <cstdint>
#include "parameter_list.hpp"
#include "symbol_table.hpp"

namespace ntt {

    class CodeGenerator {
        public:
            /* add the parameters in the symbol table and return their count */
            uint16_t compile(const ParameterList& param_list);

        private:
            SymbolTable symbol_table;
    };
}

#endif
