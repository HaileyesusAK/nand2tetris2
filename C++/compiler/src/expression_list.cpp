#include <sstream>
#include "expression_list.hpp"
#include "term_factory.hpp"

namespace ntt {

    /* expressionList : (expression (',' expression)*)?  */
    ExpressionList::ExpressionList(Tokenizer& tokenizer)  {
        /*
            expressionList is evaluated in the context of subroutine call; hence,
            by checking the value of the next token, it is possible to determine
            wether the expression list is empty or not.
        */
        if(tokenizer.peek().value() != ")") {
            expressions_.emplace_back(Expression(tokenizer));    
            while(tokenizer.has_token() && tokenizer.peek().value() == ",") {
                commas_.emplace_back(tokenizer.consume_symbol(","));
                expressions_.emplace_back(Expression(tokenizer));    
            }
        }
    }

    std::string ExpressionList::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<expressionList>", level);

        if(!expressions_.empty()) {
            oss << expressions_[0].to_xml(level + 1);
            for(size_t i = 1; i < expressions_.size(); ++i) {
                oss << JackFragment::to_xml(commas_[i - 1], level + 1);
                oss << expressions_[i].to_xml(level + 1);
            }
        }

        oss << JackFragment::get_line("</expressionList>", level);

        return oss.str();
    }

    const std::vector<Expression>& ExpressionList::expressions() const {
        return expressions_;
    }

}
