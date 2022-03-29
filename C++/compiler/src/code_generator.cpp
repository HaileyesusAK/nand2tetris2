#include <sstream>
#include "code_generator.hpp"

namespace ntt {

    const SymbolTable& CodeGenerator::symbol_table() const {
        return symbol_table_;
    }

    uint16_t CodeGenerator::compile(const ParameterList& param_list) {
        auto parameters = param_list.parameters();
        for(const auto& [type, name] : parameters) {
            if(!symbol_table_.insert(name.value(), type.value(), SymbolKind::ARGUMENT))
                throw std::runtime_error("duplicate parameter at " + name.pos());
        }

        return parameters.size();
    }

    uint16_t CodeGenerator::compile(const SubroutineVarDec& var_dec) {
        const auto& type = var_dec.type();
        auto names = var_dec.names();
        for(const auto& name : names) {
            if(!symbol_table_.insert(name.value(), type.value(), SymbolKind::LOCAL))
                throw std::runtime_error("duplicate variable at " + name.pos());
        }

        return names.size();
    }

    std::string CodeGenerator::compile(const IntegerTerm& term) {
        std::ostringstream oss;
        oss << "push constant " << term.token().value() << std::endl;
        return oss.str();
    }

    std::string CodeGenerator::compile(const StringTerm& term) {
        std::ostringstream oss;
        oss << "push constant " << term.token().value().size() << std::endl;
        oss << "call String.new 1" << std::endl;

        for(auto c : term.token().value()) {
            oss << "push constant " << static_cast<uint16_t>(c) << std::endl;
            oss << "call String.appendChar 2" << std::endl; // First argument is for 'this' of the string object
        }

        return oss.str();
    }

}
