#ifndef __FUNCTION_CALL_TERM_H__
#define __FUNCTION_CALL_TERM_H__

#include <string>
#include "token.hpp"
#include "tokenizer.hpp"
#include "expression_list.hpp"

namespace ntt {

    /* represents a function call */
    class FunctionCall : public JackFragment {
        public:
            FunctionCall(Tokenizer& tokenizer);

            std::string to_xml(size_t level = 0) const override;

            const Token& name() const;

            const std::vector<Expression>& expressions() const;

        private:
            Token func_name_;
            Token left_parenthesis_;
            ExpressionList expression_list_;
            Token right_parenthesis_;
    };
}

#endif
