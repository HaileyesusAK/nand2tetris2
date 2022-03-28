#include "code_generator.hpp"

namespace ntt {

    uint16_t CodeGenerator::compile(const ParameterList& param_list) {
        return param_list.parameters().size();
    }
}
