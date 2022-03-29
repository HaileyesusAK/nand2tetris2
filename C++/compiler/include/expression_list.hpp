#ifndef __EXPRESSION_LIST_H__
#define __EXPRESSION_LIST_H__

#include <memory>
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

            const std::vector<Expression>& expressions() const;

        private:
            std::vector<Expression> expressions_;
            std::vector<Token> commas_;
    };
}

#endif
