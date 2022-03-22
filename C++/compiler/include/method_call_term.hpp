#ifndef __METHOD_CALL_TERM_H__
#define __METHOD_CALL_TERM_H__

#include <string>
#include "token.hpp"
#include "tokenizer.hpp"
#include "expression_list.hpp"

namespace ntt {

    class MethodCallTerm : public Term {
        public:
            MethodCallTerm(Tokenizer& tokenizer);

            std::string to_xml(size_t level = 0) const override;

        private:
            Token var_name_;
            Token dot_;
            Token method_name_;
            Token left_parenthesis_;
            ExpressionList expression_list_;
            Token right_parenthesis_;
    };
}

#endif
