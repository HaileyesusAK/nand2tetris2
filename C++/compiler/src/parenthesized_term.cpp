#include <sstream>
#include <string>
#include "parenthesized_term.hpp"

namespace ntt {

    ParenthesizedTerm::ParenthesizedTerm(Tokenizer& tokenizer)
        : left_parenthesis_(tokenizer.consume_symbol("(")),
          expression_(Expression(tokenizer)),
          right_parenthesis_(tokenizer.consume_symbol(")"))
    {}

    std::string ParenthesizedTerm::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<term>", level);
        oss << left_parenthesis_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << expression_.to_xml(level + 1);
        oss << right_parenthesis_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << JackFragment::get_line("</term>", level);

        return oss.str();
    }
}
