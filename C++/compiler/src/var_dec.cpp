#include <unordered_set>
#include <sstream>
#include "var_dec.hpp"

namespace ntt {

    VarDec::TrailingVariable::TrailingVariable(Tokenizer& tokenizer)
        : comma(tokenizer.consume_symbol(",")),
          var_name(tokenizer.consume_identifier())
    {}

    VarDec::VarDec(Tokenizer& tokenizer)
        : var_(tokenizer.consume_keyword({"var"})),
          type_(tokenizer.consume_type()),
          first_var_name_(tokenizer.consume_identifier()),
          trailing_var_names_(VarDec::parse_trailing_variables_(tokenizer)),
          semicolon_(tokenizer.consume_symbol(";"))
    {}

    VarDec::TrailingVariables VarDec::parse_trailing_variables_(Tokenizer& tokenizer) {
        VarDec::TrailingVariables variables;

        while(tokenizer.has_token() && tokenizer.peek().value() != ";")
            variables.emplace_back(TrailingVariable(tokenizer));
        
        return variables;
    }

    std::string VarDec::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<varDec>", level);
        oss << var_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << type_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << first_var_name_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        for(const auto& [comma, var_name] : trailing_var_names_) {
            oss << comma.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
            oss << var_name.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        }
        oss << semicolon_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << JackFragment::get_line("</varDec>", level);

        return oss.str();
    }

}
