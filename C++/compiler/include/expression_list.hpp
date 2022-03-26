#ifndef __EXPRESSION_LIST_H__
#define __EXPRESSION_LIST_H__

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include "expression.hpp"
#include "tokenizer.hpp"
#include "token.hpp"

namespace ntt {

    /* represents an expression list */
    class ExpressionList : public JackFragment {
        public:
            ExpressionList(Tokenizer&);

            std::string to_xml(size_t level = 0) const override;

        private:
            struct TrailingExpression {
                Token comma;
                Expression expression;

                TrailingExpression(Tokenizer&);
            };

            using List = std::optional<std::pair<Expression, std::vector<TrailingExpression>>>;
            List expression_list_ = std::nullopt;
    };
}

#endif
