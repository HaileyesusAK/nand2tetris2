#include <sstream>
#include "subroutine_call_term.hpp"

namespace ntt {
    /* term: functionCall */
    SubroutineCallTerm::SubroutineCallTerm(Tokenizer& tokenizer)
        : call_(tokenizer)
    {}

    Term::Type SubroutineCallTerm::get_type() const {
        return Term::Type::SUBROUTINE_CALL;
    }

    std::string SubroutineCallTerm::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<term>", level);
        oss << call_.to_xml(level + 1);
        oss << JackFragment::get_line("</term>", level);

        return oss.str();
    }
}
