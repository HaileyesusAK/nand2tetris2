#include <sstream>
#include <unordered_set>
#include "var_dec.hpp"

namespace ntt {

    /*
        variableDec  : type varName(','varName)*';'
        type    : 'int' | 'char' | 'boolean' | className
        varName : identifier
        className : identifier
    */
    VarDec::VarDec(Tokenizer& tokenizer) : type_(tokenizer.consume_type()) {
        names_.emplace_back(tokenizer.consume_identifier());

        while(tokenizer.has_token() && tokenizer.peek().value() != ";") {
            commas_.emplace_back(tokenizer.consume_symbol(","));
            names_.emplace_back(tokenizer.consume_identifier());
        }
        semicolon_ = tokenizer.consume_symbol(";");
    }

    std::string VarDec::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::to_xml(type_, level);
        oss << JackFragment::to_xml(names_[0], level);
        for(size_t i = 0; i < commas_.size(); ++i) {
            oss << JackFragment::to_xml(commas_[i], level);
            oss << JackFragment::to_xml(names_[i+1], level);
        }
        oss << JackFragment::to_xml(semicolon_, level);

        return oss.str();
    }

    const Token& VarDec::type() const {
        return type_;
    }

    const std::vector<Token>& VarDec::names() const {
        return names_;
    }

}
