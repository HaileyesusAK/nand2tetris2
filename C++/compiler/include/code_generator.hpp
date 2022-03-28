#ifndef __CODE_GENERATOR_H__
#define __CODE_GENERATOR_H__

#include <cstdint>
#include "parameter_list.hpp"
#include "symbol_table.hpp"

namespace ntt {

    class CodeGenerator {
        public:

            /* comile the parameters and return their count */
            uint16_t compile(const ParameterList& param_list);
    };
}

#endif
