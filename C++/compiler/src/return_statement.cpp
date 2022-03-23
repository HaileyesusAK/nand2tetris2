#include <optional>
#include <sstream>
#include "return_statement.hpp"

namespace ntt {

    ReturnStatement::ReturnStatement(Tokenizer& tokenizer)
        : return_(tokenizer.consume_keyword({"return"})),
          expression_(ReturnStatement::parse_expression(tokenizer)),
          semicolon_(tokenizer.consume_symbol(";"))
    {}

    std::string ReturnStatement::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<returnStatement>", level);
        oss << return_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        if(expression_.has_value())
            oss << expression_.value().to_xml(level + 1);
        oss << semicolon_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << JackFragment::get_line("</returnStatement>", level);

        return oss.str();
    }

    std::optional<Expression> ReturnStatement::parse_expression(Tokenizer& tokenizer) {

        if(tokenizer.peek().value() != ";") {
            return Expression(tokenizer);
        }

        return std::nullopt;
    }

}
