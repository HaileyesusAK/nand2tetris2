#include "string_term.hpp"

namespace ntt {

    StringTerm::StringTerm(Tokenizer& tokenizer)
        : token_(tokenizer.consume_string()) {}

    std::string StringTerm::to_xml(size_t level) const {
        return Term::to_xml_(token_, level);
    }
}
