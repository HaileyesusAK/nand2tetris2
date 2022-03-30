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
            Expression(Tokenizer&);

            std::string to_xml(size_t level = 0) const override;

            const std::vector<std::unique_ptr<Term>>& terms() const;

            const std::vector<Token>& ops() const;

        private:
            std::vector<std::unique_ptr<Term>> terms_;
            std::vector<Token> ops_;
    };
}

#endif
