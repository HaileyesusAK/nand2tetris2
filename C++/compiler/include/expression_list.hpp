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

    class ExpressionList : public JackFragment {

        public:
            ExpressionList(Tokenizer&); 
            std::string to_xml(size_t level = 0) const override; 

        private:
            std::unique_ptr<Expression> first_expression_;
            std::vector<std::pair<Token, std::unique_ptr<Expression>>> trailing_expressions_;
    };
}

#endif
