#include "keyword_term.hpp"

namespace ntt {

    /* keywordConstant : 'true' | 'false' | 'null' | 'this' */
    KeywordTerm::KeywordTerm(Tokenizer& tokenizer)
        : token_(tokenizer.consume_keyword({"true", "false", "null", "this"})) {}

    Term::Type KeywordTerm::get_type() const {
        return Term::Type::KEYWORD;
    }

    std::string KeywordTerm::to_xml(size_t level) const {
        return Term::to_xml_(token_, level);
    }
}
