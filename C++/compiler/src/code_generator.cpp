#include "code_generator.hpp"

namespace ntt {

    uint16_t CodeGenerator::compile(const ParameterList& param_list) {
        auto parameters = param_list.parameters();
        for(const auto& [type, name] : parameters) {
            symbol_table_.insert(name.value(), type.value(), SymbolKind::ARGUMENT);
        }

        return parameters.size();
    }
    
    const SymbolTable& CodeGenerator::symbol_table() const {
        return symbol_table_;
    }
}
