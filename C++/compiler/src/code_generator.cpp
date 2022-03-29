#include <unordered_map>
#include "code_generator.hpp"

namespace ntt {

    const SymbolTable& CodeGenerator::symbol_table() const {
        return symbol_table_;
    }
    
    const std::vector<std::string>& CodeGenerator::vm_commands() const {
        return vm_commands_;
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

    void CodeGenerator::compile(const Expression& expression) {
        static const std::unordered_map<std::string, std::string> vm_ops {
            {"/", "call Math.divide 2"}, {"*", "call Math.multiply 2"}, {"+", "add"},
            {"-", "sub"}, {"&", "and"}, {"|", "or"}, {"<", "lt"}, {">", "gt"}
        };

        compile(expression.first_term());

        for(const auto& [op, term] : expression.trailing_terms()) {
            compile(term);
            vm_commands_.emplace_back(vm_ops.at(op.value()));
        }
    }

    void CodeGenerator::compile(const std::unique_ptr<Term>& term) {
        switch(term->get_type()) {
            case Term::Type::ARRAY:
                compile(static_cast<const ArrayTerm&>(*term));
            break;

            case Term::Type::IDENTIFIER:
                compile(static_cast<const IdentifierTerm&>(*term));
            break;

            case Term::Type::INTEGER:
                compile(static_cast<const IntegerTerm&>(*term));
            break;

            case Term::Type::KEYWORD:
                compile(static_cast<const KeywordTerm&>(*term));
            break;

            case Term::Type::STRING:
                compile(static_cast<const StringTerm&>(*term));
            break;

            default:
            break;
        }
    }

    void CodeGenerator::compile(const ArrayTerm& term) {
        try {
            const auto& entry = symbol_table_.get_entry(term.identifier().value());

            // evaluate the index expression
            compile(term.expression());

            // get the array's base address
            vm_commands_.emplace_back("push " + CodeGenerator::segment(entry.kind) + " " + std::to_string(entry.index));

            // evaluate address of the accessed element
            vm_commands_.emplace_back("add");

            // set that's pointer to the address of the element
            vm_commands_.emplace_back("pop pointer 1");
        }
        catch(std::out_of_range&) {
            throw std::runtime_error("undeclared identifier at " + term.identifier().pos());
        }
    }

    void CodeGenerator::compile(const IdentifierTerm& term) {
        try {
            const auto& entry = symbol_table_.get_entry(term.token().value());
            vm_commands_.emplace_back("push " + CodeGenerator::segment(entry.kind) + " " + std::to_string(entry.index));
        }
        catch(std::out_of_range&) {
            throw std::runtime_error("undeclared identifier at " + term.token().pos());
        }
    }

    void CodeGenerator::compile(const IntegerTerm& term) {
        vm_commands_.emplace_back("push constant " + term.token().value());
    }

    void CodeGenerator::compile(const KeywordTerm& term) {
        if(term.token().value() == "false" || term.token().value() == "null")
            vm_commands_.emplace_back("push constant 0");
        else if(term.token().value() == "true") {
            vm_commands_.emplace_back("push constant 1");
            vm_commands_.emplace_back("neg");   // true is stored as all ones, i.e, -1
        }
        else // this
            vm_commands_.emplace_back("push pointer 0");
    }


    void CodeGenerator::compile(const StringTerm& term) {
        vm_commands_.emplace_back("push constant " + std::to_string(term.token().value().size()));
        vm_commands_.emplace_back("call String.new 1");

        for(auto c : term.token().value()) {
            vm_commands_.emplace_back("push constant " + std::to_string(static_cast<uint16_t>(c)));

            // First argument is for 'this' of the string object
            vm_commands_.emplace_back("call String.appendChar 2");
        }
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
