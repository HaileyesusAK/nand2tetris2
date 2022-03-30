#include <sstream>
#include "subroutine_var_dec.hpp"

namespace ntt {

    /* varDec  : 'var' variableDec */
    SubroutineVarDec::SubroutineVarDec(Tokenizer& tokenizer)
        : var_(tokenizer.consume_keyword({"var"})),
          var_dec_(tokenizer)
    {}

    std::string SubroutineVarDec::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<varDec>", level);
        oss << JackFragment::to_xml(var_, level + 1);
        oss << var_dec_.to_xml(level + 1);
        oss << JackFragment::get_line("</varDec>", level);

        return oss.str();
    }

    const Token& SubroutineVarDec::type() const {
        return var_dec_.type();
    }

    const std::vector<Token>& SubroutineVarDec::names() const {
        return var_dec_.names();
    }
}
