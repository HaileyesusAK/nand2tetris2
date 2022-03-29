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

    std::string CodeGenerator::compile(const Expression& expression) {
        static const std::unordered_map<std::string, std::string> vm_ops {
            {"/", "call Math.divide 2"}, {"*", "call Math.multiply 2"}, {"+", "add"},
            {"-", "sub"}, {"&", "and"}, {"|", "or"}, {"<", "lt"}, {">", "gt"}
        };

        std::ostringstream oss;

        oss << compile(expression.first_term());

        for(const auto& [op, term] : expression.trailing_terms()) {
            oss << compile(term);
            oss << vm_ops.at(op.value());
        }

        return oss.str();
    }

    std::string CodeGenerator::compile(const std::unique_ptr<Term>& term) {
        switch(term->get_type()) {
            case Term::Type::ARRAY:
                return compile(static_cast<const ArrayTerm&>(*term)); break;

            case Term::Type::IDENTIFIER:
                return compile(static_cast<const IdentifierTerm&>(*term)); break;

            case Term::Type::INTEGER:
                return compile(static_cast<const IntegerTerm&>(*term)); break;

            case Term::Type::KEYWORD:
                return compile(static_cast<const KeywordTerm&>(*term)); break;

            case Term::Type::STRING:
                return compile(static_cast<const StringTerm&>(*term)); break;

            default:
                return "";
        }

        return "";
    }

    std::string CodeGenerator::compile(const ArrayTerm& term) {
        std::ostringstream oss;
        try {
            const auto& entry = symbol_table_.get_entry(term.identifier().value());

            // evaluate the index expression
            oss << compile(term.expression());

            // get the array's base address
            oss << "push " << CodeGenerator::segment(entry.kind) << " " << entry.index << std::endl;

            // evaluate address of the accessed element
            oss << "add" << std::endl;

            // set that's pointer to the address of the element
            oss << "pop pointer 1" << std::endl;
        }
        catch(std::out_of_range&) {
            throw std::runtime_error("undeclared identifier at " + term.identifier().pos());
        }

        return oss.str();
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
            oss << "push " << CodeGenerator::segment(entry.kind) << " " << entry.index << std::endl;
        }
        catch(std::out_of_range&) {
            throw std::runtime_error("undeclared identifier at " + term.token().pos());
        }

        return oss.str();
    }

    std::string CodeGenerator::segment(const SymbolKind& kind) {
        switch(kind) {
            case SymbolKind::LOCAL: return "local";
            case SymbolKind::STATIC: return "static";
            case SymbolKind::ARGUMENT: return "argument";
            case SymbolKind::FIELD: return "this";
            default: return "";
        }
    }
}
