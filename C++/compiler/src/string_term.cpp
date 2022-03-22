#include <string>
#include "string_term.hpp"
#include "tokenizer.hpp"

namespace ntt {

    StringTerm::StringTerm(Tokenizer& tokenizer) : token_(tokenizer.consume_string()) {}

    std::string StringTerm::to_xml() const {
        return Term::to_xml_(token_);
    }
}
