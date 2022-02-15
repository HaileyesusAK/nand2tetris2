#include <fstream>
#include <unordered_map>
#include <variant>
#include "code_generator.hpp"
#include "syntax_tree.hpp"
#include "symbol_table.hpp"


namespace ntt {

    void CodeGenerator::write(std::ofstream& ofs) {
        for(const auto& vm_command : vm_commands_)
            ofs << vm_command << std::endl;
    }

    /*
        classVarDec : ('static' | 'field') type varName(','varName)* ';'
    */
    void CodeGenerator::compile_class_var_dec(const SyntaxTree& tree) {
        // assert(tree.gen_type() == CodeGeneratorType::CLASS_VAR_DEC);

        auto kind = std::get<Token>(tree.children()[0]).value() == "static" ? SymbolKind::STATIC : SymbolKind::FIELD;
        const auto& type = std::get<Token>(tree.children()[1]).value();

        auto begin = tree.children().begin() + 2;  // iterator to the first identifer 
        auto end = tree.children().end() - 1;  // iterator to the semicolon 
        for(auto it = begin; it < end; it += 2) {
            const auto& term_tree = std::get<Tree>(*it);
            const auto& var_name = std::get<Token>(term_tree->children()[0]).value();
            symbol_table_.insert(var_name, type, kind);
        }
    }

    /*
        parameterList : ((type varName)(',' type varName)*)?
    */
    void CodeGenerator::compile_parameter_list(const SyntaxTree& tree) {
        // assert(tree.gen_type() == CodeGeneratorType::PARAMETER_LIST);
        for(auto it = tree.children().begin(); it < tree.children().end(); it += 3) {
            const auto& type = std::get<Token>(*it).value();
            const auto& var_name = std::get<Token>(*(it + 1)).value();
            symbol_table_.insert(var_name, type, SymbolKind::ARGUMENT);
        }
    }

    /*
        varDec  : 'var' type varName(','varName)*';'
        type    : 'int' | 'char' | 'boolean' | className
        varName : identifier
        className : identifier

    */
    void CodeGenerator::compile_var_dec(const SyntaxTree& tree) {
        // assert(tree.gen_type() == CodeGeneratorType::VAR_DEC);

        const auto& type = std::get<Token>(tree.children()[1]).value();

        auto begin = tree.children().begin() + 2;  // iterator to the first identifer 
        auto end = tree.children().end() - 1;  // iterator to the semicolon 

        for(auto it = begin; it < end; it += 2) {
            const auto& term_tree = std::get<Tree>(*it);
            const auto& var_name = std::get<Token>(term_tree->children()[0]).value();
            symbol_table_.insert(var_name, type, SymbolKind::LOCAL);
        }
    }

    /*
        term            : integerConstant | stringConstant | keywordConstant | varName |
                          varName '[' expresion ']' | subroutineCall | '(' expression ')' | unaryOp term
        keywordConstant : 'true' | 'false' | 'null' | 'this'
        subroutineCall  : subroutineName '(' expressionList ')' |
                          (className | varName) '.' subroutineName '(' expressionList ')'
        unaryOP         : '-' | '~'
        subroutineName  : identifier
        varName         : identifier
        className       : identifier
    */
    void CodeGenerator::compile_term(const SyntaxTree& tree) {
        switch(tree.gen_type()) {
            case CodeGeneratorType::INTEGER_TERM:
            {
                const auto& token = std::get<Token>(tree.children().at(0));
                vm_commands_.push_back("push constant " + token.value());
            }
            break;

            case CodeGeneratorType::KEYWORD_TERM:
            {
                const auto& token = std::get<Token>(tree.children().at(0));
                if(token.value() == "true") {
                    vm_commands_.push_back("push constant 1");
                    vm_commands_.push_back("neg");  // true is represented by all ones, i.e., -1
                }
                else if(token.value() == "false" || token.value() == "null")
                    vm_commands_.push_back("push constant 0");
                else
                    vm_commands_.push_back("push pointer 0");
            }
            break;

            case CodeGeneratorType::STRING_TERM:
            {
                const auto& token = std::get<Token>(tree.children().at(0));
                vm_commands_.push_back("push constant " + token.value().size());
                vm_commands_.push_back("call String.new 1");

                for(auto c : token.value()) {
                    vm_commands_.push_back("push constant " + std::to_string(static_cast<uint16_t>(c)));
                    vm_commands_.push_back("call String.appendChar 2");    // first argument is for 'this' of the string object
                }

            }
            break;

            case CodeGeneratorType::IDENTIFIER_TERM:
            {
                const auto& identifer = std::get<Token>(tree.children()[0]).value();
                const auto& entry = symbol_table_.get_entry(identifer);
                switch(entry.kind) {
                    case SymbolKind::FIELD:
                        vm_commands_.push_back("push this " + std::to_string(entry.index));
                    break;

                    case SymbolKind::STATIC:
                        vm_commands_.push_back("push static " + std::to_string(entry.index));
                    break;

                    case SymbolKind::ARGUMENT:
                        vm_commands_.push_back("push argument " + std::to_string(entry.index));
                    break;

                    case SymbolKind::LOCAL:
                        vm_commands_.push_back("push local " + std::to_string(entry.index));
                    break;
                }
            }
            break;

            case CodeGeneratorType::ARRAY_TERM:
            {
                const auto& identifer = std::get<Token>(tree.children()[0]).value();
                const auto& entry = symbol_table_.get_entry(identifer);
                switch(entry.kind) {
                    case SymbolKind::FIELD:
                        vm_commands_.push_back("push this " + std::to_string(entry.index));
                    break;

                    case SymbolKind::STATIC:
                        vm_commands_.push_back("push static " + std::to_string(entry.index));
                    break;

                    case SymbolKind::ARGUMENT:
                        vm_commands_.push_back("push argument " + std::to_string(entry.index));
                    break;

                    case SymbolKind::LOCAL:
                        vm_commands_.push_back("push local " + std::to_string(entry.index));
                    break;
                }

                auto& expression_tree = std::get<Tree>(tree.children()[2]);
                compile_expression(*expression_tree);
            }
            break;

            case CodeGeneratorType::FUNCTION_CALL_TERM:
            {
                const auto& function_name = std::get<Token>(tree.children()[0]).value();
                const auto& exp_list_tree = std::get<Tree>(tree.children()[2]);

                uint16_t n_param = 0;
                for(auto it = exp_list_tree->children().begin(); it < exp_list_tree->children().end(); it += 2) {
                    compile_expression(*(std::get<Tree>(*it)));
                    ++n_param;
                }

                vm_commands_.push_back("call " + function_name + " " + std::to_string(n_param));
            }
            break;

            case CodeGeneratorType::METHOD_CALL_TERM:
            {
                //skip class name and dot operator

                const auto& method_name = std::get<Token>(tree.children()[2]).value();
                const auto& exp_list_tree = std::get<Tree>(tree.children()[4]);

                uint16_t n_param = 1; //    +1 for the object
                for(auto it = exp_list_tree->children().begin(); it < exp_list_tree->children().end(); it += 2) {
                    compile_expression(*(std::get<Tree>(*it)));
                    ++n_param;
                }

                vm_commands_.push_back("call " + method_name + " " + std::to_string(n_param));
            }
            break;

            case CodeGeneratorType::UNARY_TERM:
            {
                const auto& op = std::get<Token>(tree.children()[0]).value();
                compile_term(*std::get<Tree>(tree.children()[1]));
                if(op == "~")
                    vm_commands_.push_back("not");
                else
                    vm_commands_.push_back("neg");
            }
            break;

            case CodeGeneratorType::PARENTHESIS_TERM:
            {
                auto& expression_tree = std::get<Tree>(tree.children()[1]);
                compile_expression(*expression_tree);
            }
            break;

            default:
            break;
        }
    }

    /*
        expression : term (op term)*
        op : '+' |  '-' |  '*' |  '/' |  '&' |  '|' |  '<' |  '>' |  '='
    */
    void CodeGenerator::compile_expression(const SyntaxTree& tree) {
        const static std::unordered_map<std::string, std::string> binary_op {
            {"+", "add"},
            {"-", "sub"},
            {"*", "call Math.multiply 2"},
            {"/", "call Math.divide 2"},
            {"&", "and"},
            {"|", "or"},
            {"<", "lt"},
            {">", "gt"},
            {"=", "eq"}
        };

        auto& term_tree = std::get<Tree>(tree.children()[0]);
        compile_term(*term_tree);

        for(auto it = tree.children().begin() + 1; it < tree.children().end(); it += 2) {
            const auto& op = binary_op.at(std::get<Token>(*it).value());
            auto& term_tree = std::get<Tree>(*(it + 1));

            compile_term(*term_tree);
            vm_commands_.push_back(op); //evaluate expression as postfix
        }   
    }
}
