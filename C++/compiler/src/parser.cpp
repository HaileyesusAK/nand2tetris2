#include <fstream>
#include <memory>
#include <stdexcept>
#include <unordered_set>
#include "parser.hpp"
#include "syntax_tree.hpp"
#include "token.hpp"
#include <iostream>
namespace ntt {

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
    Tree Parser::parse_term() {
        if(!tokenizer.has_token())
            throw NoTokenErr();

        static std::unordered_set<std::string> keywords {"true", "false", "null", "this"};
        
        auto tree = std::make_unique<SyntaxTree>("term");

        auto token = tokenizer.get();
        switch(token.type()) {
            case TokenType::INTEGER:
            case TokenType::STRING:
                tree->add_child(std::make_unique<Leaf>(token));
            break;

            case TokenType::KEYWORD:
                if(!keywords.count(token.value()))
                    throw std::runtime_error("invalid keyword constant");
                else
                    tree->add_child(std::make_unique<Leaf>(token));
            break;

            case TokenType::IDENTIFIER:
                tree->add_child(std::make_unique<Leaf>(token));
                if(tokenizer.has_token()) {
                    if(tokenizer.peek().value() == "[") { // Array expression
                        tree->add_child(std::make_unique<Leaf>(tokenizer.get()));   // [
                        tree->add_child(parse_exp()); // exp
                        tree->add_child(std::make_unique<Leaf>(tokenizer.consume_symbol("]"))); // ]
                    }
                    else if(tokenizer.peek().value() == "(") { // subroutine call
                        tree->add_child(std::make_unique<Leaf>(tokenizer.get()));   // (
                        tree->add_child(parse_exp_list()); // exp list
                        tree->add_child(std::make_unique<Leaf>(tokenizer.consume_symbol(")"))); // )
                    }
                    else if(tokenizer.peek().value() == ".") { // method call
                        tree->add_child(std::make_unique<Leaf>(tokenizer.get()));   // .
                        tree->add_child(std::make_unique<Leaf>(tokenizer.consume_identifier()));    // method name
                        tree->add_child(std::make_unique<Leaf>(tokenizer.consume_symbol("(")));   // (
                        tree->add_child(parse_exp_list()); // exp list
                        tree->add_child(std::make_unique<Leaf>(tokenizer.consume_symbol(")"))); // )
                    }
                }
            break;

            case TokenType::SYMBOL:
                if(token.value() == "(") {
                    tree->add_child(std::make_unique<Leaf>(token)); // (
                    tree->add_child(parse_exp());   // exp
                    tree->add_child(std::make_unique<Leaf>(tokenizer.get())); // )
                }
                else if (token.value() == "-" || token.value() == "~") {
                    tree->add_child(std::make_unique<Leaf>(token)); // unaryOp
                    tree->add_child(parse_term());   // term 
                }
                else
                    throw std::runtime_error("invalid symbol token");
            break;

            default:
                return nullptr;
        }

        return tree;
    }

    /*
        expression : term (op term)*
        op : '+' |  '-' |  '*' |  '/' |  '&' |  '|' |  '<' |  '>' |  '='
    */

    Tree Parser::parse_exp() {
        static std::unordered_set<std::string> ops {
            "+" ,  "-" ,  "*" ,  "/" ,  "&" ,  "|" ,  "<" ,  ">" ,  "="
        };

        if(!tokenizer.has_token())
            throw NoTokenErr();

        auto tree = std::make_unique<SyntaxTree>("expression");
        tree->add_child(parse_term());

        while(true) {
            if(!tokenizer.has_token())
                break;

            const auto& token = tokenizer.peek();
            if(!ops.count(token.value()))
                break;

            tree->add_child(std::make_unique<Leaf>(tokenizer.get()));
            tree->add_child(parse_term());
        }

        return tree;
    }

    /*
        expressionList : (expression (',' expression)*)?
    */
    Tree Parser::parse_exp_list() {
        if(!tokenizer.has_token())
            throw NoTokenErr();

        auto tree = std::make_unique<SyntaxTree>("expressionList");
        /*
            expressionList is evaluated in the context of subroutine call; hence,
            by checking the value of the next token, it is possible to determine
            wether the expression list is empty or not.
        */
        if(tokenizer.peek().value() != ")") {
            tree->add_child(parse_exp());
            while(tokenizer.has_token() && tokenizer.peek().value() == ",") {
                tree->add_child(std::make_unique<Leaf>(tokenizer.get()));   // add ,
                tree->add_child(parse_exp());
            }
        }

        return tree;
    }

    /*
        letStatement : 'let' varName ('[' expression ']')? '=' expression ';'
        varName      : identifier
    */
    Tree Parser::parse_let_statement() {
        if(!tokenizer.has_token())
            throw NoTokenErr();

        auto tree = std::make_unique<SyntaxTree>("letStatement");

        tree->add_child(std::make_unique<Leaf>(tokenizer.consume_keyword("let")));
        tree->add_child(std::make_unique<Leaf>(tokenizer.consume_identifier()));
        if(tokenizer.peek().value() == "[") {
            tree->add_child(std::make_unique<Leaf>(tokenizer.get()));   // [
            tree->add_child(parse_exp());   // expression
            tree->add_child(std::make_unique<Leaf>(tokenizer.consume_symbol("]")));   // ]
        }
        tree->add_child(std::make_unique<Leaf>(tokenizer.consume_symbol("=")));   // =
        tree->add_child(parse_exp());   // expression
        tree->add_child(std::make_unique<Leaf>(tokenizer.consume_symbol(";")));   // ; 

        return tree;
    }
}
