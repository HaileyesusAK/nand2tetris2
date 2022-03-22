#include <sstream>
#include <string>
#include "array_term.hpp"

namespace ntt {

    ArrayTerm::ArrayTerm(Tokenizer& tokenizer)
        : var_name_(tokenizer.consume_identifier()),
          left_bracket_(tokenizer.consume_symbol("[")),
          expression_(Expression(tokenizer)),
          right_bracket_(tokenizer.consume_symbol("]"))
    {}

    std::string ArrayTerm::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<term>", level);
        oss << var_name_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << left_bracket_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << expression_.to_xml(level + 1);
        oss << right_bracket_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << JackFragment::get_line("</term>", level);

        return oss.str();
    }
}
