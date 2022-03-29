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

    std::string CodeGenerator::compile(const KeywordTerm& term) {
        std::ostringstream oss;
        if(term.token().value() == "false" || term.token().value() == "null")
            oss << "push constant 0" << std::endl;
        else if(term.token().value() == "true") {
            oss << "push constant 1" << std::endl;
            oss << "neg" << std::endl;
        }
        else
            oss << "push pointer 0" << std::endl;

        return oss.str();
    }

    std::string CodeGenerator::compile(const IdentifierTerm& term) {
        std::ostringstream oss;
        try {
            const auto& entry = symbol_table_.get_entry(term.token().value());
            switch(entry.kind) {
                case SymbolKind::LOCAL:
                    oss << "push local ";
                break;

                case SymbolKind::STATIC:
                    oss << "push static ";
                break;

                case SymbolKind::ARGUMENT:
                    oss << "push argument ";
                break;

                case SymbolKind::FIELD:
                    oss << "push this ";
                break;
            }
            oss << entry.index << std::endl;
        }
        catch(std::out_of_range&) {
            throw std::runtime_error("undeclared identifier at " + term.token().pos());
        }

        return oss.str();
    }

}
