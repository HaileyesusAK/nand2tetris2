#ifndef __SUBROUTINE_CALL_TERM_H__
#define __SUBROUTINE_CALL_TERM_H__

#include <string>
#include "token.hpp"
#include "tokenizer.hpp"
#include "expression_list.hpp"

namespace ntt {

    class SubroutineCallTerm : public Term {
        public:
            SubroutineCallTerm(Tokenizer& tokenizer);

            std::string to_xml(size_t level = 0) const override;

        private:
            Token var_name_;
            Token left_parenthesis_;
            ExpressionList expression_list_;
            Token right_parenthesis_;
    };
}

#endif
