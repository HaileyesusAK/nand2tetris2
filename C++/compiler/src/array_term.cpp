#include <sstream>
#include "array_term.hpp"

namespace ntt {

    /* term: varName '[' expresion ']' */
    ArrayTerm::ArrayTerm(Tokenizer& tokenizer)
        : var_name_(tokenizer.consume_identifier()),
          left_bracket_(tokenizer.consume_symbol("[")),
          expression_(Expression(tokenizer)),
          right_bracket_(tokenizer.consume_symbol("]"))
    {}

    std::string ArrayTerm::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<term>", level);
        oss << JackFragment::to_xml(var_name_, level + 1);
        oss << JackFragment::to_xml(left_bracket_, level + 1);
        oss << expression_.to_xml(level + 1);
        oss << JackFragment::to_xml(right_bracket_, level + 1);
        oss << JackFragment::get_line("</term>", level);

        return oss.str();
    }
}
