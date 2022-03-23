#include <sstream>
#include <stdexcept>
#include "subroutine_dec.hpp"

namespace ntt {

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
        oss << routine_type_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << return_type_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << name_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << left_parenthesis_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << parameter_list_.to_xml(level + 1);
        oss << right_parenthesis_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << body_.to_xml(level + 1);
        oss << JackFragment::get_line("</subroutineDec>", level);

        return oss.str();
    }
}

