#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <memory>
#include <vector>
#include "term.hpp"
#include "types.hpp"

namespace ntt {
    class Expression {
        public:
            Expression(std::unique_ptr<Term> term);

            Expression(std::unique_ptr<Expression> expression)
                : terms_(std::move(expression->terms_)),
                  ops_(std::move(expression->ops_)) {}

            void add_term(const BinaryOp& op, std::unique_ptr<Term> term);

        private:
            std::vector<std::unique_ptr<Term>> terms_;
            std::vector<BinaryOp> ops_;
    };
}
#endif
