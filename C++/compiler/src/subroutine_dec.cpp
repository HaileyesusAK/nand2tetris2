#include <sstream>
#include <stdexcept>
#include "subroutine_dec.hpp"

namespace ntt {

    /*
        subroutineDec : ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody
        subroutineName : identifier
    */
    SubroutineDec::SubroutineDec(Tokenizer& tokenizer)
        : routine_type_(tokenizer.consume_keyword({"constructor", "method", "function"})),
          return_type_(SubroutineDec::get_return_type_(tokenizer)),
          name_(tokenizer.consume_identifier()),
          left_parenthesis_(tokenizer.consume_symbol("(")),
          parameter_list_(ParameterList(tokenizer)),
          right_parenthesis_(tokenizer.consume_symbol(")")),
          body_(SubroutineBody(tokenizer))
    {}

    Token SubroutineDec::get_return_type_(Tokenizer& tokenizer) {
        try {
            return tokenizer.consume_type();
        }
        catch(std::exception&) {
            return tokenizer.consume_keyword({"void"});
        }
    }

    std::string SubroutineDec::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<subroutineDec>", level);
        oss << JackFragment::to_xml(routine_type_, level + 1);
        oss << JackFragment::to_xml(return_type_, level + 1);
        oss << JackFragment::to_xml(name_, level + 1);
        oss << JackFragment::to_xml(left_parenthesis_, level + 1);
        oss << parameter_list_.to_xml(level + 1);
        oss << JackFragment::to_xml(right_parenthesis_, level + 1);
        oss << body_.to_xml(level + 1);
        oss << JackFragment::get_line("</subroutineDec>", level);

        return oss.str();
    }
}

