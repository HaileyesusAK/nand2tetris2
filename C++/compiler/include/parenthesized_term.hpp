#ifndef __PARENTHESIZED_TERM_H__
#define __PARENTHESIZED_TERM_H__

#include <string>
#include "token.hpp"
#include "tokenizer.hpp"
#include "expression.hpp"

namespace ntt {

    /* represents a parenthesized term */
    class ParenthesizedTerm : public Term {
        public:
            ParenthesizedTerm(Tokenizer& tokenizer);

            std::string to_xml(size_t level = 0) const override;

        private:
            Token left_parenthesis_;
            Expression expression_;
            Token right_parenthesis_;
    };
}

#endif
