#include <unordered_map>
#include "code_generator.hpp"

namespace ntt {

    CodeGenerator::CodeGenerator(const std::string& class_name) : class_name_(class_name) {}

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

            case Term::Type::PARENTHESIZED_TERM:
                compile(static_cast<const ParenthesizedTerm&>(*term));
            break;

            case Term::Type::STRING:
                compile(static_cast<const StringTerm&>(*term));
            break;

            case Term::Type::SUBROUTINE_CALL:
                compile(static_cast<const SubroutineCallTerm&>(*term));
            break;

            case Term::Type::METHOD_CALL:
                compile(static_cast<const MethodCallTerm&>(*term));
            break;

            case Term::Type::UNARY_OP:
                compile(static_cast<const UnaryOpTerm&>(*term));
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

    void CodeGenerator::compile(const ParenthesizedTerm& term) {
        compile(term.expression());
    }

    void CodeGenerator::compile(const StringTerm& term) {
        // push the length of the string
        vm_commands_.emplace_back("push constant " + std::to_string(term.token().value().size()));

        // call the builtin static method to allocate space for the string
        vm_commands_.emplace_back("call String.new 1");

        for(auto c : term.token().value()) {
            vm_commands_.emplace_back("push constant " + std::to_string(static_cast<uint16_t>(c)));

            // call String.appendChar with pointer to the string object and the ascii value of the character
            vm_commands_.emplace_back("call String.appendChar 2");
        }
    }

    void CodeGenerator::compile(const SubroutineCallTerm& term) {
        // the first argument must be pointer to the current object
        vm_commands_.emplace_back("push pointer 0");

        // push the arguments
        for(const auto& expression : term.expressions())
            compile(expression);

        auto n_args = term.expressions().size() + 1; // +1 is for the current object

        // This is to disambiguate the same method name that is defined in another class
        auto subroutine_name = class_name_ + "." + term.name().value();

        vm_commands_.emplace_back("call " + subroutine_name + " " + std::to_string(n_args));
    }

    void CodeGenerator::compile(const MethodCallTerm& term) {
        auto n_args = term.expressions().size();
        std::string class_name;

        try {   // assuming the variable is a class type
            const auto& entry = symbol_table_.get_entry(term.variable().value());

            // first argument is always address of the object
            vm_commands_.emplace_back("push " + CodeGenerator::segment(entry.kind) + " " + std::to_string(entry.index));
            class_name = entry.type;
            n_args++;   // take into account also the address of the object
        }
        catch(std::out_of_range&) { // the call is to a static function
            class_name = term.variable().value();
        }

        // push the arguments
        for(const auto& expression : term.expressions())
            compile(expression);

        // qualify the method's name with the class name of the variable
        auto method_name = class_name + "." + term.method_name().value();

        vm_commands_.emplace_back("call " + method_name + " " + std::to_string(n_args));
    }

    void CodeGenerator::compile(const UnaryOpTerm& term) {
        compile(term.term());
        if(term.op().value() == "~")
            vm_commands_.emplace_back("not");
        else
            vm_commands_.emplace_back("neg");
    }

    void CodeGenerator::compile(const std::unique_ptr<Statement>& statement) {
        switch(statement->type()) {
            case Statement::Type::DO:
                compile(static_cast<const DoStatement&>(*statement));
            break;

            case Statement::Type::IF:
                compile(static_cast<const IfStatement&>(*statement));
            break;

            case Statement::Type::LET:
                compile(static_cast<const LetStatement&>(*statement));
            break;

            case Statement::Type::RETURN:
                compile(static_cast<const ReturnStatement&>(*statement));
            break;

            case Statement::Type::WHILE:
                compile(static_cast<const WhileStatement&>(*statement));
            break;

            default:
            break;
        }
    }

    void CodeGenerator::compile(const DoStatement& statement) {
        try {
            compile(std::get<MethodCallTerm>(statement.call_term()));
        }
        catch(std::bad_variant_access&) {
            compile(std::get<SubroutineCallTerm>(statement.call_term()));
        }
    }

    void CodeGenerator::compile(const IfStatement& statement) {
        static size_t label_count = 0;

        label_count++;

        std::string label_suffix { std::to_string(label_count) + "_" + class_name_ };
        std::string else_label { "Else" + label_suffix};
        std::string end_label { "End_If" + label_suffix};

        compile(statement.expression());

        // it is easier to jump after negating the expression's result
        vm_commands_.emplace_back("not");

        // jump to the else part or to the end if the expression evaluates to false
        if(statement.else_statements().has_value())
            vm_commands_.emplace_back("if-goto " + else_label);
        else
            vm_commands_.emplace_back("if-goto " + end_label);

        // the expression evaluates to true; hence, compile the statements in the if block
        for(const auto& inner_statement : statement.if_statements())
            compile(inner_statement);

        if(statement.else_statements().has_value()) {
            // arrives at this line only if the 'if' part is executed
            vm_commands_.emplace_back("goto " + end_label);

            // if the expression evaluates to false, control jumps directly to here
            vm_commands_.emplace_back("label " + else_label);

            for(const auto& inner_statement : statement.else_statements().value().get())
                compile(inner_statement);
        }

        vm_commands_.emplace_back("label " + end_label);

        // necessary to revert back since the inner statements could contain another while statement
        label_count++;
    }

    void CodeGenerator::compile(const LetStatement& statement) {
        const auto& entry = symbol_table_.get_entry(statement.variable().value());

        if(statement.index_expression().has_value()) {  // assignment to an array element
            compile(statement.index_expression().value());
            vm_commands_.emplace_back("push " + CodeGenerator::segment(entry.kind) + " " + std::to_string(entry.index));
            vm_commands_.emplace_back("add");   // evaluate address of the array element
            vm_commands_.emplace_back("pop pointer 1"); // set 'that' to the address of the element
            compile(statement.assignment_expression());
            vm_commands_.emplace_back("pop that 0"); // assign the expression's result in the element
        }
        else {
            compile(statement.assignment_expression());
            vm_commands_.emplace_back("pop " + CodeGenerator::segment(entry.kind) + " " + std::to_string(entry.index));
        }
    }

    void CodeGenerator::compile(const ReturnStatement& statement) {
        if(statement.expression().has_value())
            compile(statement.expression().value());

        vm_commands_.emplace_back("return");
    }

    void CodeGenerator::compile(const WhileStatement& statement) {
        static size_t label_count = 0;

        label_count++;

        std::string label_suffix { std::to_string(label_count) + "_" + class_name_ };
        std::string start_label { "Begin_While" + label_suffix};
        std::string end_label { "End_While" + label_suffix};

        vm_commands_.emplace_back("label " + start_label);
        compile(statement.expression());
        vm_commands_.emplace_back("not");    // it is easier to jump after negating the expression's result
        vm_commands_.emplace_back("if-goto " + end_label);

        for(const auto& inner_statement : statement.statements())
            compile(inner_statement);

        vm_commands_.emplace_back("label " + end_label);

        // necessary to revert back since the inner statements could contain another while statement
        label_count--;
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
