#include <string>
#include "keyword_term.hpp"
#include "tokenizer.hpp"

namespace ntt {

    KeywordTerm::KeywordTerm(Tokenizer& tokenizer)
        : token_(tokenizer.consume_keyword({"true", "false", "null", "this"})) {}

    std::string KeywordTerm::to_xml() const {
        return Term::to_xml_(token_);
    }
}
