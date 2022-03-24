#include <sstream>
#include "expression_list.hpp"
#include "term_factory.hpp"

namespace ntt {

    ExpressionList::TrailingExpression::TrailingExpression(Tokenizer& tokenizer)
        : comma(tokenizer.consume_symbol(",")),
          expression(Expression(tokenizer))
    {}

    /* expressionList : (expression (',' expression)*)?  */
    ExpressionList::ExpressionList(Tokenizer& tokenizer)  {
        /*
            expressionList is evaluated in the context of subroutine call; hence,
            by checking the value of the next token, it is possible to determine
            wether the expression list is empty or not.
        */
        if(tokenizer.peek().value() != ")") {
            expression_list_ = std::make_pair<Expression, std::vector<TrailingExpression>>(Expression(tokenizer), {});

            auto& trailing_expressions = expression_list_.value().second;
            while(tokenizer.has_token() && tokenizer.peek().value() == ",")
                trailing_expressions.emplace_back(TrailingExpression(tokenizer));
        }
    }

    std::string ExpressionList::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<expressionList>", level);
        if(expression_list_.has_value()) {
            const auto& list = expression_list_.value();
            oss << list.first.to_xml(level + 1);
            for(const auto& trailing_expression : list.second) {
                oss << JackFragment::to_xml(trailing_expression.comma, level + 1);
                oss << trailing_expression.expression.to_xml(level + 1);
            }
        }

        oss << JackFragment::get_line("</expressionList>", level);

        return oss.str();
    }

}
