#include <string>
#include "keyword_term.hpp"
#include "tokenizer.hpp"

namespace ntt {

    KeywordTerm::KeywordTerm(Tokenizer& tokenizer)
        : token_(tokenizer.consume_keyword({"true", "false", "null", "this"})) {}

    std::string KeywordTerm::to_xml(size_t level) const {
        return Term::to_xml_(token_, level);
    }
}
