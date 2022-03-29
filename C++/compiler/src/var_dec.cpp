#include <sstream>
#include <unordered_set>
#include "var_dec.hpp"

namespace ntt {

    VarDec::TrailingVariable::TrailingVariable(Tokenizer& tokenizer)
        : comma(tokenizer.consume_symbol(",")),
          var_name(tokenizer.consume_identifier())
    {}

    /*
        variableDec  : type varName(','varName)*';'
        type    : 'int' | 'char' | 'boolean' | className
        varName : identifier
        className : identifier
    */
    VarDec::VarDec(Tokenizer& tokenizer)
        : type_(tokenizer.consume_type()),
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

        oss << JackFragment::to_xml(type_, level);
        oss << JackFragment::to_xml(first_var_name_, level);
        for(const auto& [comma, var_name] : trailing_var_names_) {
            oss << JackFragment::to_xml(comma, level);
            oss << JackFragment::to_xml(var_name, level);
        }
        oss << JackFragment::to_xml(semicolon_, level);

        return oss.str();
    }

    Token VarDec::type() const {
        return type_;
    }

    std::vector<Token> VarDec::names() const {
        std::vector<Token> names {first_var_name_};
        for(const auto& trailing_variable : trailing_var_names_)
            names.emplace_back(trailing_variable.var_name);
        return names;
    }

}
