#include <sstream>
#include <string>
#include "method_call_term.hpp"

namespace ntt {

    MethodCallTerm::MethodCallTerm(Tokenizer& tokenizer)
        : var_name_(tokenizer.consume_identifier()),
          dot_(tokenizer.consume_symbol(".")),
          method_name_(tokenizer.consume_identifier()),
          left_parenthesis_(tokenizer.consume_symbol("(")),
          expression_list_(ExpressionList(tokenizer)),
          right_parenthesis_(tokenizer.consume_symbol(")"))
    {}

    std::string MethodCallTerm::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<term>", level);
        oss << var_name_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << dot_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << method_name_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << left_parenthesis_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << expression_list_.to_xml(level + 1);
        oss << right_parenthesis_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << JackFragment::get_line("</term>", level);

        return oss.str();
    }
}
