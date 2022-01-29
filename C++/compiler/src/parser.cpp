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
        subroutineName  : identifier
    */
    Tree Parser::parse_term() {
        if(!tokenizer.has_token())
            throw NoTokenErr();

        static std::unordered_set<std::string> keywords {"true", "false", "null", "this"};
        auto token = tokenizer.get();
        switch(token.type()) {
            case TokenType::INTEGER:
            case TokenType::STRING:
                return std::make_unique<SyntaxTree>(token);

            case TokenType::KEYWORD:
                if(!keywords.count(token.value()))
                    throw std::runtime_error("invalid keyword constant");
                else
                    return std::make_unique<SyntaxTree>(token);

            case TokenType::IDENTIFIER:
                return std::make_unique<SyntaxTree>(token);

            default:
                return nullptr;
        }
    }
}
