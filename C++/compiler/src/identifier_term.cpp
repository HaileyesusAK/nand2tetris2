#include <string>
#include "identifier_term.hpp"
#include "tokenizer.hpp"

namespace ntt {

    IdentifierTerm::IdentifierTerm(Tokenizer& tokenizer) : token_(tokenizer.consume_identifier()) {}

    std::string IdentifierTerm::to_xml() const {
        return Term::to_xml_(token_);
    }
}
