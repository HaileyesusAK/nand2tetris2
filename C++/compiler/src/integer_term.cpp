#include <string>
#include "integer_term.hpp"
#include "tokenizer.hpp"

namespace ntt {

    IntegerTerm::IntegerTerm(Tokenizer& tokenizer) : token_(tokenizer.consume_integer()) {}

    std::string IntegerTerm::to_xml() const {
        return Term::to_xml_(token_);
    }
}
