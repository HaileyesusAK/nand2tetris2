#include <sstream>
#include "do_statement.hpp"

namespace ntt {

    /* doStatement : 'do' subroutineCall | methodCall ';' */
    DoStatement::DoStatement(Tokenizer& tokenizer)
        : do_(tokenizer.consume_keyword({"do"})),
          call_term_(DoStatement::parse_call_term_(tokenizer)),
          semicolon_(tokenizer.consume_symbol(";"))
    {}

    std::string DoStatement::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<doStatement>", level);
        oss << JackFragment::to_xml(do_, level + 1);
        oss << std::visit(CallTermVisitor{level + 1}, call_term_);
        oss << JackFragment::to_xml(semicolon_, level + 1);
        oss << JackFragment::get_line("</doStatement>", level);

        return oss.str();
    }

    DoStatement::CallTerm DoStatement::parse_call_term_(Tokenizer& tokenizer) {
        auto token = tokenizer.get();
        if(tokenizer.peek().value() == ".") {
            tokenizer.put(token);
            return MethodCallTerm(tokenizer);
        }
        else {
            tokenizer.put(token);
            return SubroutineCallTerm(tokenizer);
        }
    }

    const DoStatement::CallTerm& DoStatement::call_term() const {
        return call_term_;
    }
}
