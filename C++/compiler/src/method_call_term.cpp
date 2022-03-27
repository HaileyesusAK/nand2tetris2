#include <sstream>
#include <string>
#include "method_call_term.hpp"

namespace ntt {

    /* term: (className | varName) '.' functionCall */
    MethodCallTerm::MethodCallTerm(Tokenizer& tokenizer)
        : var_name_(tokenizer.consume_identifier()),
          dot_(tokenizer.consume_symbol(".")),
          call_(tokenizer)
    {}

    Term::Type MethodCallTerm::get_type() const {
        return Term::Type::METHOD_CALL;
    }

    std::string MethodCallTerm::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<term>", level);
        oss << JackFragment::to_xml(var_name_, level + 1);
        oss << JackFragment::to_xml(dot_, level + 1);
        oss << call_.to_xml(level + 1);
        oss << JackFragment::get_line("</term>", level);

        return oss.str();
    }
}
