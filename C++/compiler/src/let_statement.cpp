#include <sstream>
#include "let_statement.hpp"

namespace ntt {

    LetStatement::IndexExpression::IndexExpression(Tokenizer& tokenizer)
        : left_bracket(tokenizer.consume_symbol("[")),
          expression(Expression(tokenizer)),
          right_bracket(tokenizer.consume_symbol("]"))
    {}

    /*
        letStatement : 'let' varName ('[' expression ']')? '=' expression ';'
        varName      : identifier
    */
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
        oss << JackFragment::to_xml(let_, level + 1);
        oss << JackFragment::to_xml(var_name_, level + 1);

        if(index_expression_.has_value()) {
            const auto& expression = index_expression_.value();
            oss << JackFragment::to_xml(expression.left_bracket, level + 1);
            oss << expression.expression.to_xml(level + 1);
            oss << JackFragment::to_xml(expression.right_bracket, level + 1);
        }

        oss << JackFragment::to_xml(eq_, level + 1);
        oss << expression_.to_xml(level + 1);
        oss << JackFragment::to_xml(semicolon_, level + 1);
        oss << JackFragment::get_line("</letStatement>", level);

        return oss.str();
    }

    const Token& LetStatement::variable() const {
        return var_name_;
    }

    const Expression& LetStatement::assignment_expression() const {
        return expression_;
    }

}
