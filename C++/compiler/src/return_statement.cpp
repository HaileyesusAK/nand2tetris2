#include <optional>
#include <sstream>
#include "return_statement.hpp"

namespace ntt {

    /* returnStatement : 'return' expression? ';' */
    ReturnStatement::ReturnStatement(Tokenizer& tokenizer)
        : return_(tokenizer.consume_keyword({"return"})),
          expression_(ReturnStatement::parse_expression_(tokenizer)),
          semicolon_(tokenizer.consume_symbol(";"))
    {}

    std::string ReturnStatement::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<returnStatement>", level);
        oss << JackFragment::to_xml(return_, level + 1);
        if(expression_.has_value())
            oss << expression_.value().to_xml(level + 1);
        oss << JackFragment::to_xml(semicolon_, level + 1);
        oss << JackFragment::get_line("</returnStatement>", level);

        return oss.str();
    }

    std::optional<Expression> ReturnStatement::parse_expression_(Tokenizer& tokenizer) {
        if(tokenizer.peek().value() != ";") {
            return Expression(tokenizer);
        }

        return std::nullopt;
    }

}
