#include "identifier_term.hpp"
#include "integer_term.hpp"
#include "keyword_term.hpp"
#include "string_term.hpp"
#include "term.hpp"
#include "term_factory.hpp"

namespace ntt {

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
                return std::make_unique<IdentifierTerm>(tokenizer);

            default:
                return nullptr;
        }
    }
}
