#include <sstream>
#include "function_call.hpp"

namespace ntt {

    /* functionCall  : functionName '(' expressionList ')' */
    FunctionCall::FunctionCall(Tokenizer& tokenizer)
        : func_name_(tokenizer.consume_identifier()),
          left_parenthesis_(tokenizer.consume_symbol("(")),
          expression_list_(ExpressionList(tokenizer)),
          right_parenthesis_(tokenizer.consume_symbol(")"))
    {}

    std::string FunctionCall::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::to_xml(func_name_, level);
        oss << JackFragment::to_xml(left_parenthesis_, level);
        oss << expression_list_.to_xml(level);
        oss << JackFragment::to_xml(right_parenthesis_, level);

        return oss.str();
    }
}
