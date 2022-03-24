#include <sstream>
#include "class_var_dec.hpp"

namespace ntt {

    /* classVarDec : ('static' | 'field') variableDec */
    ClassVarDec::ClassVarDec(Tokenizer& tokenizer)
        : storage_class_(tokenizer.consume_keyword({"static", "field"})),
          var_dec_(tokenizer)
    {}

    std::string ClassVarDec::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<classVarDec>", level);
        oss << JackFragment::to_xml(storage_class_, level + 1);
        oss << var_dec_.to_xml(level + 1);
        oss << JackFragment::get_line("</classVarDec>", level);

        return oss.str();
    }

}
