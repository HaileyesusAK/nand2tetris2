#include <sstream>
#include "let_statement.hpp"

namespace ntt {

    LetStatement::IndexExpression::IndexExpression(Tokenizer& tokenizer)
        : left_bracket(tokenizer.consume_symbol("[")),
          expression(Expression(tokenizer)),
          right_bracket(tokenizer.consume_symbol("]"))
    {}

    LetStatement::LetStatement(Tokenizer& tokenizer)
        : let_(tokenizer.consume_keyword({"let"})),
          var_name_(tokenizer.consume_identifier()),
          index_expression_(LetStatement::parse_index_exp_(tokenizer)),
          eq_(tokenizer.consume_symbol("=")),
          expression_(Expression(tokenizer)),
          semicolon_(tokenizer.consume_symbol(";")) {
    }

    std::optional<LetStatement::IndexExpression> LetStatement::parse_index_exp_(Tokenizer& tokenizer) {
        if(tokenizer.peek().value() == "[") 
            return IndexExpression(tokenizer);

        return std::nullopt;
    }

    std::string LetStatement::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<letStatement>", level);
        oss << let_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << var_name_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;

        if(index_expression_.has_value()) {
            const auto& expression = index_expression_.value();
            oss << expression.left_bracket.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
            oss << expression.expression.to_xml(level + 1);
            oss << expression.right_bracket.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        }

        oss << eq_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << expression_.to_xml(level + 1);
        oss << semicolon_.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
        oss << JackFragment::get_line("</letStatement>", level);

        return oss.str();
    }
}
