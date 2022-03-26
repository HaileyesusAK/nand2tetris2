#include <sstream>
#include "term_factory.hpp"
#include "unaryop_term.hpp"

namespace ntt {

    /* term:    unaryOp term
       unaryOP: '-' | '~'
    */

    UnaryOpTerm::UnaryOpTerm(Tokenizer& tokenizer)
        : unary_op_(tokenizer.consume_symbol(std::unordered_set<std::string>{"~", "-"})),
          term_(TermFactory::parse(tokenizer))
    { }

    std::string UnaryOpTerm::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<term>", level);
        oss << unary_op_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << term_->to_xml(level + 1);
        oss << JackFragment::get_line("</term>", level);

        return oss.str();
    }
}
