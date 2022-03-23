#include <sstream>
#include "class.hpp"

namespace ntt {

    Class::Class(Tokenizer& tokenizer)
        : class_(tokenizer.consume_keyword({"class"})),
          name_(tokenizer.consume_identifier()),
          left_brace_(tokenizer.consume_symbol("{")),
          class_var_dec_(Class::parse_class_var_dec_(tokenizer)),
          subroutine_dec_(Class::parse_subroutine_dec_(tokenizer)),
          right_brace_(tokenizer.consume_symbol("}"))
    {}

    std::string Class::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<class>", level);

        oss << class_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << name_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << left_brace_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;

        for(const auto& class_var_dec : class_var_dec_)
            oss << class_var_dec.to_xml(level + 1);

        for(const auto& subroutine_dec: subroutine_dec_)
            oss << subroutine_dec.to_xml(level + 1);

        oss << right_brace_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;

        oss << JackFragment::get_line("</class>", level);

        return oss.str();
    }

    std::vector<ClassVarDec> Class::parse_class_var_dec_(Tokenizer& tokenizer) {
        std::vector<ClassVarDec> dec_;

        while(tokenizer.has_token() && (tokenizer.peek().value() == "static" || tokenizer.peek().value() == "field"))
            dec_.emplace_back(tokenizer);

        return dec_;
    }

    std::vector<SubroutineDec> Class::parse_subroutine_dec_(Tokenizer& tokenizer) {
        std::vector<SubroutineDec> dec_;

        while(tokenizer.has_token() && tokenizer.peek().value() != "}")
            dec_.emplace_back(tokenizer);

        return dec_;
    }
}
