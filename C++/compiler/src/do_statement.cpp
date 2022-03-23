#include <sstream>
#include "do_statement.hpp"

namespace ntt {

    DoStatement::DoStatement(Tokenizer& tokenizer)
        : do_(tokenizer.consume_keyword({"do"})),
          call_term_(SubroutineCallTerm(tokenizer)),
          semicolon_(tokenizer.consume_symbol(";"))
    {}

    std::string DoStatement::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<doStatement>", level);
        oss << do_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << call_term_.to_xml(level + 1);
        oss << semicolon_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << JackFragment::get_line("</doStatement>", level);

        return oss.str();
    }
}
