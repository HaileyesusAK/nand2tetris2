#include "array_term.hpp"
#include "identifier_term.hpp"
#include "integer_term.hpp"
#include "keyword_term.hpp"
#include "method_call_term.hpp"
#include "parenthesized_term.hpp"
#include "string_term.hpp"
#include "subroutine_call_term.hpp"
#include "term.hpp"
#include "term_factory.hpp"
#include "unaryop_term.hpp"

namespace ntt {

    /*
        term            : integerConstant | stringConstant | keywordConstant | varName |
                          varName '[' expresion ']' | subroutineCall | methodCall |
                          '(' expression ')' | unaryOp term
        keywordConstant : 'true' | 'false' | 'null' | 'this'
        subroutineCall  : functionCallsubroutineName '(' expressionList ')' |
        methodCall      : (className | varName) '.' functionCall
        functionCall    : functionName '(' expressionList ')'
        unaryOP         : '-' | '~'
        subroutineName  : identifier
        varName         : identifier
        className       : identifier
    */
    std::unique_ptr<Term> TermFactory::parse(Tokenizer& tokenizer) {
        if(!tokenizer.has_token())
            return nullptr;

        auto token = tokenizer.peek();
        switch(token.type()) {
            case TokenType::INTEGER:
                return std::make_unique<IntegerTerm>(tokenizer);

            case TokenType::KEYWORD:
                return std::make_unique<KeywordTerm>(tokenizer);

            case TokenType::STRING:
                return std::make_unique<StringTerm>(tokenizer);

            case TokenType::IDENTIFIER:
            {
                token = tokenizer.get();
                if(tokenizer.has_token()) {
                    if(tokenizer.peek().value() == "[") {
                        tokenizer.put(token);
                        return std::make_unique<ArrayTerm>(tokenizer);
                    }
                    else if(tokenizer.peek().value() == "(") {
                        tokenizer.put(token);
                        return std::make_unique<SubroutineCallTerm>(tokenizer);
                    }
                    else if(tokenizer.peek().value() == ".") {
                        tokenizer.put(token);
                        return std::make_unique<MethodCallTerm>(tokenizer);
                    }
                }

                tokenizer.put(token);
                return std::make_unique<IdentifierTerm>(tokenizer);
            }

            case TokenType::SYMBOL:
                if(token.value() == "(")
                    return std::make_unique<ParenthesizedTerm>(tokenizer);
                else if (token.value() == "-" || token.value() == "~")
                    return std::make_unique<UnaryOpTerm>(tokenizer);
                else
                    throw std::runtime_error("invalid symbol token");
        }

        return nullptr;
    }

}
