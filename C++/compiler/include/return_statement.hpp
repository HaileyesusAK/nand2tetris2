#ifndef __RETURN_STATEMENT_H__
#define __RETURN_STATEMENT_H__

#include <optional>
#include <string>
#include "expression.hpp"
#include "jack_fragment.hpp"
#include "token.hpp"
#include "tokenizer.hpp"

namespace ntt {

    class ReturnStatement : public JackFragment {
        public:
            ReturnStatement(Tokenizer&);

            std::string to_xml(size_t level = 0) const override;

        private:
            Token return_;

            std::optional<Expression> expression_;

            Token semicolon_;

            std::optional<Expression> parse_expression(Tokenizer&);
    };
}

#endif
