#include <sstream>
#include <string>
#include "parenthesized_term.hpp"

namespace ntt {

    /* term: '(' expression ')' */
    ParenthesizedTerm::ParenthesizedTerm(Tokenizer& tokenizer)
        : left_parenthesis_(tokenizer.consume_symbol("(")),
          expression_(Expression(tokenizer)),
          right_parenthesis_(tokenizer.consume_symbol(")"))
    {}

    std::string ParenthesizedTerm::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<term>", level);
        oss << JackFragment::to_xml(left_parenthesis_, level + 1);
        oss << expression_.to_xml(level + 1);
        oss << JackFragment::to_xml(right_parenthesis_, level + 1);
        oss << JackFragment::get_line("</term>", level);

        return oss.str();
    }
}
