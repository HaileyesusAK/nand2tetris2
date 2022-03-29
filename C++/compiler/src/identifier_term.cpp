#include "identifier_term.hpp"

namespace ntt {

    IdentifierTerm::IdentifierTerm(Tokenizer& tokenizer)
        : token_(tokenizer.consume_identifier()) {}

    Term::Type IdentifierTerm::get_type() const {
        return Term::Type::IDENTIFIER;
    }

    std::string IdentifierTerm::to_xml(size_t level) const {
        return Term::to_xml_(token_, level);
    }

    const Token& IdentifierTerm::token() const {
        return token_;
    }
}
