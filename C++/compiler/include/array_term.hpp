#ifndef __ARRAY_TERM_H__
#define __ARRAY_TERM_H__

#include <string>
#include "token.hpp"
#include "tokenizer.hpp"
#include "expression.hpp"

namespace ntt {

    /* represents an array term */
    class ArrayTerm : public Term {
        public:
            ArrayTerm(Tokenizer& tokenizer);

            std::string to_xml(size_t level = 0) const override;

            Type get_type() const override;

            const Token& identifier() const;

            const Expression& expression() const;

        private:
            Token var_name_;
            Token left_bracket_;
            Expression expression_;
            Token right_bracket_;
    };
}

#endif
