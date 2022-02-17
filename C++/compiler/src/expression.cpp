#include <memory>
#include "expression.hpp"
#include "term.hpp"
#include "types.hpp"

namespace ntt {
    Expression::Expression(std::unique_ptr<Term> term) {
        terms_.push_back(std::move(term));
    }

    /*
    Expression::Expression(Expression&& expression) {
        e
    }
    */

    void Expression::add_term(const BinaryOp& op, std::unique_ptr<Term> term) {
        ops_.push_back(std::move(op));
        terms_.push_back(std::move(term));
    }
}
