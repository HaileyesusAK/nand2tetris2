#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "term.hpp"
#include "tokenizer.hpp"
#include "token.hpp"

namespace ntt {

    /* represents an expression */
    class Expression : public JackFragment {
        public:
            using TrailingTerms = std::vector<std::pair<Token, std::unique_ptr<Term>>>;
            Expression(Tokenizer&); 

            std::string to_xml(size_t level = 0) const override; 

            const std::unique_ptr<Term>& first_term() const;
            
            const TrailingTerms& trailing_terms() const;

        private:
            std::unique_ptr<Term> first_term_;
            TrailingTerms trailing_terms_;
    };
}

#endif
