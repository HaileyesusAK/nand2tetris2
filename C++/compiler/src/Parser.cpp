#include <fstream>
#include <memory>
#include <stdexcept>
#include "Parser.hpp"
#include "syntax_tree.hpp"
#include "token.hpp"
#include "terms.hpp"
#include "types.hpp"

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
    std::unique_ptr<Term> parser::parse_term() {
        if(!tokenizer_.has_token())
            throw NoTokenErr();

        static const std::unordered_map<std::string, Keyword> keywords {
            {"true", Keyword::TRUE},
            {"false", Keyword::FALSE},
            {"null", Keyword::NULL_},
            {"this", Keyword::THIS}
        };

        auto token = tokenizer_.get();
        switch(token.type()) {
            case TokenType::INTEGER:
                return std::make_unique<IntegerTerm>(static_cast<uint16_t>(std::stoul(token.value())));
            break;

            case TokenType::STRING:
                return std::make_unique<StringTerm>(token.value());
            break;

            case TokenType::KEYWORD:
                if(!keywords.count(token.value()))
                    throw std::runtime_error("invalid keyword constant");
                else
                    return std::make_unique<KeywordTerm>(keywords.at(token.value()));

             case TokenType::IDENTIFIER:
                tree->add_child(token);
                if(tokenizer.has_token()) {
                    if(tokenizer.peek().value() == "[") { // Array expression
                        tokenizer.get();
                        auto exp = parse_exp();
                        tokenizer.consume_symbol("]");
                        
                        return std::make_unique<std::string, std::unique_ptr<Expression>(token.value(), exp);
                    }
            break;

            default:
                return nullptr;
        }
        return nullptr;
    }
}
