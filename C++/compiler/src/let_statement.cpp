#include <sstream>
#include "let_statement.hpp"

namespace ntt {

    LetStatement::LetStatement(Tokenizer& tokenizer)
        : let_(tokenizer.consume_keyword({"let"})),
          var_name_(tokenizer.consume_identifier()),
          index_expression_(LetStatement::parse_index_exp(tokenizer)),
          eq_(tokenizer.consume_symbol("=")),
          expression_(Expression(tokenizer)),
          semicolon_(tokenizer.consume_symbol(";")) {
    }

    LetStatement::IndexExpression LetStatement::parse_index_exp(Tokenizer& tokenizer) {
        if(tokenizer.peek().value() == "[") {
            auto left_bracket = tokenizer.get();
            auto expression = Expression(tokenizer);
            auto right_bracket = tokenizer.consume_symbol("]");
            return std::make_tuple(std::move(left_bracket), std::move(expression), std::move(right_bracket));
        }

        return std::nullopt;
    }

    std::string LetStatement::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<letStatement>", level);
        oss << let_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << var_name_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        if(index_expression_.has_value()) {
            oss << std::get<0>(index_expression_.value()).to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
            oss << std::get<1>(index_expression_.value()).to_xml(level + 1);
            oss << std::get<2>(index_expression_.value()).to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        }

        oss << eq_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << expression_.to_xml(level + 1);
        oss << semicolon_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << JackFragment::get_line("</letStatement>", level);

        return oss.str();
    }
}
