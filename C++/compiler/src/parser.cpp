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
                        tree->add_child(std::make_unique<Leaf>(tokenizer.get()));   // add [
                        tree->add_child(parse_exp()); // add exp
                        tree->add_child(std::make_unique<Leaf>(tokenizer.get())); // add ]
                    }
                }
            break;

            case TokenType::SYMBOL:
                if(token.value() == "(") {
                    tree->add_child(std::make_unique<Leaf>(token)); // add (
                    tree->add_child(parse_exp());   // add exp
                    tree->add_child(std::make_unique<Leaf>(tokenizer.get())); // add )
                }
                else if (token.value() == "-" || token.value() == "~") {
                    tree->add_child(std::make_unique<Leaf>(token)); // add unaryOp
                    tree->add_child(parse_term());   // add term 
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
            "+" ,  "-" ,  "*" ,  "/" ,  "&" ,  "," ,  "<" ,  ">" ,  "="
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
}
