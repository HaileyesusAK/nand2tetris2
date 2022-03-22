#include <sstream>
#include "expression_list.hpp"
#include "term_factory.hpp"

namespace ntt {

    ExpressionList::ExpressionList(Tokenizer& tokenizer)  {
        /*
            expressionList is evaluated in the context of subroutine call; hence,
            by checking the value of the next token, it is possible to determine
            wether the expression list is empty or not.
        */
        if(tokenizer.peek().value() != ")") {
            first_expression_ = std::make_unique<Expression>(tokenizer);
            while(tokenizer.has_token() && tokenizer.peek().value() == ",") {
                auto comma = tokenizer.get();
                trailing_expressions_.emplace_back(comma, new Expression(tokenizer));
            }
        }
    }

    std::string ExpressionList::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<expressionList>", level);
        if(first_expression_) {
            oss << first_expression_->to_xml(level + 1);
            for(const auto& [comma, expression] : trailing_expressions_) {
                oss << comma.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
                oss << expression->to_xml(level + 1);
            }
        }

        oss << JackFragment::get_line("</expressionList>", level);

        return oss.str();
    }

}
