#include <sstream>
#include "subroutine_var_dec.hpp"

namespace ntt {

    SubroutineVarDec::SubroutineVarDec(Tokenizer& tokenizer)
        : var_(tokenizer.consume_keyword({"var"})),
          var_dec_(tokenizer)
    {}

    std::string SubroutineVarDec::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<varDec>", level);
        oss << var_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << var_dec_.to_xml(level + 1);
        oss << JackFragment::get_line("</varDec>", level);

        return oss.str();
    }
}
